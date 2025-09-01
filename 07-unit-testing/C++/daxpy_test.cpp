#include <assert.h>
#include <cmath>
#include <iostream>
#include <chrono>

#include <gtest/gtest.h>


void daxpy(int n, double a, double *x, double *y) {
    if (n <= 0 || a == 0.0) {
        return;
    }

    for (int i = 0; i < n; i++) {
        y[i] += a * x[i];
    }
}

void daxpy_unrolled(int n, double a, double *x, double *y) {

    if (n <= 0 || a == 0.0) {
        return;
    }

    int m = n % 4;
    for (int i = 0; i < m; i++) {
        y[i] += a * x[i];
    }

    for (int i = m; i < n; i += 4) {
        y[i] += a * x[i];
        y[i + 1] += a * x[i + 1];
        y[i + 2] += a * x[i + 2];
        y[i + 3] += a * x[i + 3];
    }
}



TEST(DaxpyTest, SmallArray) {
    const int n = 5;
    const double a = 2.0;
    double x[n] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double y[n] = {5.0, 4.0, 3.0, 2.0, 1.0};
    double expected_y[n] = {7.0, 8.0, 9.0, 10.0, 11.0};

    daxpy(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}

TEST(DaxpyTest, ZeroLength) {
    const int n = 0;
    const double a = 2.0;
    double x[n] = {};
    double y[n] = {};
    double expected_y[n] = {};

    daxpy(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}

TEST(DaxpyTest, NegativeLength) {
    const int n = -5;
    const double a = 2.0;
    double* x = nullptr;
    double* y = nullptr;

    // Expect no crash or change in y
    daxpy(n, a, x, y);
    SUCCEED();
}

TEST(DaxpyTest, ZeroScalar) {
    const int n = 5;
    const double a = 0.0;
    double x[n] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double y[n] = {5.0, 4.0, 3.0, 2.0, 1.0};
    double expected_y[n] = {5.0, 4.0, 3.0, 2.0, 1.0};

    daxpy(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}

TEST(DaxpyTest, UnrolledSmallArray) {
    const int n = 5;
    const double a = 2.0;
    double x[n] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double y[n] = {5.0, 4.0, 3.0, 2.0, 1.0};
    double expected_y[n] = {7.0, 8.0, 9.0, 10.0, 11.0};

    daxpy_unrolled(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}
TEST(DaxpyTest, UnrolledZeroLength) {
    const int n = 0;
    const double a = 2.0;
    double x[n] = {};
    double y[n] = {};
    double expected_y[n] = {};

    daxpy_unrolled(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}

TEST(DaxpyTest, UnrolledNegativeLength) {
    const int n = -5;
    const double a = 2.0;
    double* x = nullptr;
    double* y = nullptr;

    daxpy_unrolled(n, a, x, y);
    SUCCEED();
}

TEST(DaxpyTest, UnrolledZeroScalar) {
    const int n = 5;
    const double a = 0.0;
    double x[n] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double y[n] = {5.0, 4.0, 3.0, 2.0, 1.0};
    double expected_y[n] = {5.0, 4.0, 3.0, 2.0, 1.0};

    daxpy_unrolled(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}

TEST(DaxpyTest, DecimalScalar) {
    const int n = 6;
    const double a = 1.5;
    double y[n] = {0.5, -1.2, 3.3, 4.4, 5.5, -6.6};
    double x[n] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0};
    double expected_y[n] = {15.5, 28.8, 48.3, 64.4, 80.5, 83.4};

    daxpy(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}

TEST(DaxpyTest, UnrolledDecimalScalar) {
    const int n = 6;
    const double a = 2.75;
    double x[n] = {0.5, -1.2, 3.3, 4.4, 5.5, -6.6};
    double y[n] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0};
    double expected_y[n] = {
        10.0 + 2.75 * 0.5,
        20.0 + 2.75 * -1.2,
        30.0 + 2.75 * 3.3,
        40.0 + 2.75 * 4.4,
        50.0 + 2.75 * 5.5,
        60.0 + 2.75 * -6.6
    };

    daxpy_unrolled(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}

TEST(DaxpyTest, LargeArrayUnrolled) {
    const int n = 100;
    const double a = 1.1;
    double x[n], y[n], expected_y[n];
    for (int i = 0; i < n; i++) {
        x[i] = i * 0.1;
        y[i] = i * 0.2;
        expected_y[i] = y[i] + a * x[i];
    }

    daxpy_unrolled(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}

TEST(DaxpyTest, LargeArray) {
    const int n = 100;
    const double a = 1.1;
    double x[n], y[n], expected_y[n];
    for (int i = 0; i < n; i++) {
        x[i] = i * 0.1;
        y[i] = i * 0.2;
        expected_y[i] = y[i] + a * x[i];
    }

    daxpy(n, a, x, y);

    for (int i = 0; i < n; i++) {
        EXPECT_NEAR(y[i], expected_y[i], 1e-15);
    }
}
