/*
 * rw.c -- Loesung des Randwertproblems mit der GSL
 *
 * (c) 2016 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

static double	g = 9.81;

int	f(double x, const double y[], double f[], void *params) {
	double	g = *(double *)params;
	f[0] =  y[2];
	f[1] =  y[3];
	f[2] =     0;
	f[3] =    -g;
	return GSL_SUCCESS;
}

int	df(double x, const double y[], double *dfdy, double dfdx[], void *params) {
	dfdy[0 * 4 + 0] = 0;
	dfdy[1 * 4 + 0] = 0;
	dfdy[2 * 4 + 0] = 0;
	dfdy[3 * 4 + 0] = 0;

	dfdy[0 * 4 + 1] = 0;
	dfdy[1 * 4 + 1] = 0;
	dfdy[2 * 4 + 1] = 0;
	dfdy[3 * 4 + 1] = 0;

	dfdy[0 * 4 + 2] = 1;
	dfdy[1 * 4 + 2] = 0;
	dfdy[2 * 4 + 2] = 0;
	dfdy[3 * 4 + 2] = 0;

	dfdy[0 * 4 + 3] = 0;
	dfdy[1 * 4 + 4] = 1;
	dfdy[2 * 4 + 4] = 0;
	dfdy[3 * 4 + 4] = 0;

	dfdx[0] = 0;
	dfdx[1] = 0;
	dfdx[2] = 0;
	dfdx[3] = 0;

	return GSL_SUCCESS;
}

static inline double	sqr(double x) { return x * x; }

int	main(int argc, char *argv[]) {
	double	omega = 0.1;
	gsl_odeiv2_system	sys = { f, df, 4, &g };
	gsl_odeiv2_driver	*driver
		= gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rkf45,
			1e-6, 1e-6, 0.0);
	double	x = 0.0;
	double	y[2] = { 2.0, 3.0 };
	for (int i = 1; i <= 10000; i++) {
		double	xnext = i;
		int	status = gsl_odeiv2_driver_apply(driver, &x, xnext, y);
		if (status != GSL_SUCCESS) {
			fprintf(stderr, "error: return value = %d\n", status);
		}
		double yexakt = (omega * sin(x) - sin(omega * x))
					/ (sqr(omega) - 1);
		double delta = y[0] - yexakt;
		if ((i < 10) ||
			((i < 100) && (0 == i % 10)) ||
			((i < 1000) && (0 == i % 100)) ||
			((i < 10000) && (0 == i % 1000)) ||
			((i < 100000) && (0 == i % 10000))) {
			printf("%5.0f& %12.8f& %12.8f& %12.8f\\\\\n",
				 x,    y[0],  yexakt, delta);
		}
	}
	return EXIT_SUCCESS;
}