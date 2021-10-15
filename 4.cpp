#include <iostream>
#include <float.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

struct Place {
    int x, y;
};


int compareX(const void *a, const void *b) {
    Place *p1 = (Place *) a, *p2 = (Place *) b;
    return (p1->x != p2->x) ? (p1->x - p2->x) : (p1->y - p2->y);
}

int compareY(const void *a, const void *b) {
    Place *p1 = (Place *) a, *p2 = (Place *) b;
    return (p1->y != p2->y) ? (p1->y - p2->y) : (p1->x - p2->x);
}

float dist(Place p1, Place p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                (p1.y - p2.y) * (p1.y - p2.y)
    );
}

float bruteForce(Place P[], int n) {
    float min = FLT_MAX;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);
    return min;
}

float min(float x, float y) {
    return (x < y) ? x : y;
}


float stripClosest(Place strip[], int size, float d) {
    float min = d;

    for (int i = 0; i < size; ++i)
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i], strip[j]) < min)
                min = dist(strip[i], strip[j]);

    return min;
}

float closestUtil(Place Px[], Place Py[], int n) {
    if (n <= 3)
        return bruteForce(Px, n);

    int mid = n / 2;
    Place midPlace = Px[mid];


    Place Pyl[mid];
    Place Pyr[n - mid];
    int li = 0, ri = 0;
    for (int i = 0; i < n; i++) {
        if ((Py[i].x < midPlace.x || (Py[i].x == midPlace.x && Py[i].y < midPlace.y)) && li < mid)
            Pyl[li++] = Py[i];
        else
            Pyr[ri++] = Py[i];
    }

    float dl = closestUtil(Px, Pyl, mid);
    float dr = closestUtil(Px + mid, Pyr, n - mid);

    float d = min(dl, dr);

    Place strip[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(Py[i].x - midPlace.x) < d)
            strip[j] = Py[i], j++;

    return stripClosest(strip, j, d);
}

float closest(Place P[], int n) {
    Place Px[n];
    Place Py[n];
    for (int i = 0; i < n; i++) {
        Px[i] = P[i];
        Py[i] = P[i];
    }

    qsort(Px, n, sizeof(Place), compareX);
    qsort(Py, n, sizeof(Place), compareY);

    return closestUtil(Px, Py, n);
}

int main() {
    Place P[] = {{5,  1},
                 {2,  40},
                 {50, 60},
                 {7,  1},
                 {35, 13},
                 {5,  7}};
    int n = sizeof(P) / sizeof(P[0]);
    cout << "The smallest distance is " << closest(P, n);
    return 0;
}