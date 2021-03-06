/** dude, is my code constant time?
 *
 * This file measures the execution time of a given function many times with
 * different inputs and performs a Welch's t-test to determine if the function
 * runs in constant time or not. This is essentially leakage detection, and
 * not a timing attack.
 *
 * Notes:
 *
 *  - the execution time distribution tends to be skewed towards large
 *    timings, leading to a fat right tail. Most executions take little time,
 *    some of them take a lot. We try to speed up the test process by
 *    throwing away those measurements with large cycle count. (For example,
 *    those measurements could correspond to the execution being interrupted
 *    by the OS.) Setting a threshold value for this is not obvious; we just
 *    keep the x% percent fastest timings, and repeat for several values of x.
 *
 *  - the previous observation is highly heuristic. We also keep the uncropped
 *    measurement time and do a t-test on that.
 *
 *  - we also test for unequal variances (second order test), but this is
 *    probably redundant since we're doing as well a t-test on cropped
 *    measurements (non-linear transform)
 *
 *  - as long as any of the different test fails, the code will be deemed
 *    variable time.
 *
 */

#include "fixture.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "constant.h"
#include "percentile.h"
#include "random.h"
#include "ttest.h"

#define number_tests                                                    \
    (1 /* first order uncropped */ + number_percentiles /* cropped */ + \
     1 /* second order uncropped */)
#define enough_measurements 10000
#define number_percentiles 100
#define test_tries 10

extern const size_t chunk_size;
extern const size_t number_measurements;

static int64_t percentiles[number_percentiles] = {0};

/* threshold values for Welch's t-test */
#define t_threshold_bananas                                                  \
    500                         /* Test failed with overwhelming probability \
                                 */
#define t_threshold_moderate 10 /* Test failed */

static void __attribute__((noreturn)) die(void)
{
    exit(111);
}

// fill percentiles.
// the exponential tendency is mean to approximately match
// the measurements distribution.
static void prepare_percentiles(int64_t *ticks)
{
    for (size_t i = 0; i < number_percentiles; i++) {
        percentiles[i] = percentile(
            ticks, 1 - (pow(0.5, 10 * (double) (i + 1) / number_percentiles)),
            number_measurements);
    }
}

static void update_statistics(t_ctx *ctx, int64_t *exec_times, uint8_t *classes)
{
    for (size_t i = 0; i < number_measurements; i++) {
        int64_t difference = exec_times[i];
        /* Cpu cycle counter overflowed or dropped measurement */
        if (difference <= 0) {
            continue;
        }
        /* do a t-test on the execution time */
        t_push(&ctx[0], difference, classes[i]);

        // do a t-test on cropped execution times, for several cropping
        // thresholds.
        for (size_t crop_index = 0; crop_index < number_percentiles;
             crop_index++) {
            if (difference < percentiles[crop_index]) {
                t_push(&ctx[crop_index + 1], difference, classes[i]);
            }
        }

        // do a second-order test (only if we have more than 10000
        // measurements). Centered product pre-processing.
        if (ctx[0].n[0] > 10000) {
            double centered = (double) difference - ctx[0].mean[classes[i]];
            t_push(&ctx[1 + number_percentiles], centered * centered,
                   classes[i]);
        }
    }
}

// which t-test yields max t value?
static int max_test(t_ctx *ctx)
{
    int ret = 0;
    double max = 0;
    for (size_t i = 0; i < number_tests; i++) {
        if (ctx[i].n[0] > enough_measurements) {
            double x = fabs(t_compute(&ctx[i]));
            if (max < x) {
                max = x;
                ret = i;
            }
        }
    }
    return ret;
}

static bool report(t_ctx *ctx)
{
    int mt = max_test(ctx);
    double max_t = fabs(t_compute(&ctx[mt]));
    double number_traces_max_t = ctx[mt].n[0] + ctx[mt].n[1];
    double max_tau = max_t / sqrt(number_traces_max_t);

    printf("\033[A\033[2K");
    printf("meas: %7.2lf M, ", (number_traces_max_t / 1e6));
    if (number_traces_max_t < enough_measurements) {
        printf("not enough measurements (%.0f still to go).\n",
               enough_measurements - number_traces_max_t);
        return false;
    }

    /*
     * max_t: the t statistic value
     * max_tau: a t value normalized by sqrt(number of measurements).
     *          this way we can compare max_tau taken with different
     *          number of measurements. This is sort of "distance
     *          between distributions", independent of number of
     *          measurements.
     * (5/tau)^2: how many measurements we would need to barely
     *            detect the leak, if present. "barely detect the
     *            leak" = have a t value greater than 5.
     */
    printf("max t: %+7.2f, max tau: %.2e, (5/tau)^2: %.2e.\n", max_t, max_tau,
           (double) (5 * 5) / (double) (max_tau * max_tau));

    if (max_t > t_threshold_bananas) {
        return false;
    } else if (max_t > t_threshold_moderate) {
        return false;
    } else { /* max_t < t_threshold_moderate */
        return true;
    }
}

static bool doit(t_ctx *ctx, int mode)
{
    int64_t *exec_times = calloc(number_measurements, sizeof(int64_t));
    uint8_t *classes = calloc(number_measurements, sizeof(uint8_t));
    uint8_t *input_data =
        calloc(number_measurements * chunk_size, sizeof(uint8_t));

    if (!exec_times || !classes || !input_data) {
        die();
    }

    prepare_inputs(input_data, classes);
    measure(exec_times, input_data, mode);

    // we compute the percentiles only if they are not filled yet
    if (percentiles[number_percentiles - 1] == 0) {
        prepare_percentiles(exec_times);
    }

    update_statistics(ctx, exec_times, classes);
    bool ret = report(ctx);

    free(exec_times);
    free(classes);
    free(input_data);
    close_randombytes();

    return ret;
}

static void init_once(t_ctx *ctx)
{
    init_dut();
    for (int i = 0; i < number_tests; i++) {
        t_init(&ctx[i]);
    }
}

bool is_insert_tail_const(void)
{
    bool result = false;

    t_ctx *ctx = malloc(number_tests * sizeof(t_ctx));
    for (int cnt = 0; cnt < test_tries; ++cnt) {
        printf("Testing insert_tail...(%d/%d)\n\n", cnt, test_tries);
        init_once(ctx);
        for (int i = 0; i < enough_measurements / (number_measurements) + 1;
             ++i)
            result = doit(ctx, 0);
        printf("\033[A\033[2K\033[A\033[2K");
        if (result == true)
            break;
    }
    free(ctx);

    return result;
}

bool is_size_const(void)
{
    bool result = false;

    t_ctx *ctx = malloc(number_tests * sizeof(t_ctx));
    for (int cnt = 0; cnt < test_tries; ++cnt) {
        printf("Testing size...(%d/%d)\n\n", cnt, test_tries);
        init_once(ctx);
        for (int i = 0; i < enough_measurements / (number_measurements) + 1;
             ++i)
            result = doit(ctx, 1);
        printf("\033[A\033[2K\033[A\033[2K");
        if (result == true)
            break;
    }
    free(ctx);

    return result;
}
