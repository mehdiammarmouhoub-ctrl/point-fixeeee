#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double polynome(double x, double a, double b, double c) {
    return a * x * x + b * x + c;
}

double g_point_fixe(double x, double a, double b, double c) {
    if (fabs(a) < 1e-10) return x;  // évite division par 0
    return -0.5 * (b * x + c) / a;  // transformation f(x)=0 → x=g(x)
}

double methode_point_fixe(double x0, double tol, int max_iter, double A, double B, double C) {
    printf("f(x0=%.6f) = %.6f\n", x0, polynome(x0, A, B, C));

    double x1 = g_point_fixe(x0, A, B, C);
    printf("g(x0)    = %.6f\n", x1);

    if (fabs(x1 - x0) < tol) {
        printf("Convergence en 1 iteration!\n");
        return x1;
    }

    printf("\nIterations Point Fixe:\n");
    printf("%4s %12s %12s %10s\n", "It", "x_k", "x_{k+1}", "|err|");
    printf("-------------------------------------------\n");

    double x_prev = x0;
    int iter = 0;
    while (iter < max_iter) {
        printf("%4d %12.8f %12.8f ", ++iter, x0, x1);
        double err = fabs(x1 - x0);
        printf("%9.2e", err);

        if (err < tol) {
            printf(" *OK*\n");
            break;
        }
        printf("\n");
        x_prev = x0;
        x0 = x1;
        x1 = g_point_fixe(x0, A, B, C);
    }

    printf("\nRACINE Point Fixe: %.10f\n", x1);
    printf("Vérif f(racine) = %.2e\n", polynome(x1, A, B, C));
    return x1;
}

void dessiner_graph_point_fixe(double xmin, double xmax, double A, double B, double C, double racine) {
    int largeur = 70, hauteur = 20;
    double ymin = 1e9, ymax = -1e9;

    for (double x = xmin; x <= xmax; x += 0.05) {
        double fy = polynome(x, A, B, C);
        double gy = g_point_fixe(x, A, B, C);
        if (fy < ymin) ymin = fy; if (fy > ymax) ymax = fy;
        if (gy < ymin) ymin = gy; if (gy > ymax) ymax = gy;
    }

    printf("\n=== GRAPH: f(x) et g(x) ===\n");
    printf("RACINE où f(x)=0 et x=g(x): ^ = %.6f\n\n", racine);

    for (int iy = hauteur; iy >= 0; iy--) {
        double vy = ymax - (ymax - ymin) * iy / hauteur;
        printf("%7.2f|", vy);

        for (int ix = 0; ix < largeur; ix++) {
            double vx = xmin + (xmax - xmin) * ix / largeur;
            double fy = polynome(vx, A, B, C);
            double gy = g_point_fixe(vx, A, B, C);

            int plot_f = fabs(fy - vy) < fabs(ymax - ymin) / hauteur;
            int plot_g = fabs(gy - vy) < fabs(ymax - ymin) / hauteur;

            if (fabs(vx - racine) < (xmax - xmin)/largeur * 0.5) {
                printf("^");  // RACINE
            } else if (plot_f && plot_g) {
                printf("+");  // f(x)=g(x)=0
            } else if (plot_f) {
                printf("f");
            } else if (plot_g) {
                printf("g");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("       +------------------------------------------------\n");
    printf("       %.2f         %.2f         %.2f\n", xmin, (xmin+xmax)/2, xmax);
}

int main() {
    double x0, tol, A, B, C, xmin, xmax;

    printf("=== METHODE POINT FIXE ===\n\n");
    printf("Domaine graph [xmin,xmax]: ");
    scanf("%lf %lf", &xmin, &xmax); fflush(stdin);

    printf("x0 initial: ");
    scanf("%lf", &x0); fflush(stdin);

    printf("Tolerance: ");
    scanf("%lf", &tol); fflush(stdin);

    printf("A B C (f(x)=Ax²+Bx+C): ");
    scanf("%lf %lf %lf", &A, &B, &C);

    double racine = methode_point_fixe(x0, tol, 30, A, B, C);

    dessiner_graph_point_fixe(xmin, xmax, A, B, C, racine);

    printf("\nPoint Fixe OK! |g''(x)| < 1 autour de la racine.\n");
    printf("Appuyez sur Entree...");
    int c; do c = getchar(); while (c != '\\n');
    return 0;
}
