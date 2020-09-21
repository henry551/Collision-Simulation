#include <bits/stdc++.h>

using namespace std;
struct ball { //structure to store properties of object
    double sx, sy, vx, vy, ax, ay, m, I, w, a;
    //displacement x, displacement y, velocity x, velocity y, acceleration x, acceleration y,
    //mass, moment of inertia, angular velocity, angular acceleration
} obj[2]; //initialize two objects

double k = 12000; //spring constant
double r = 0.03; //radius
double miu = 1.16; //coefficient of friction (rubber on rubber)

//function to calculate distance between points
double dist(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); //basic trigonometry calculation
}

int main() {
    //gather user input
    scanf("%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", &obj[0].sx, &obj[0].sy, &obj[0].vx, &obj[0].vy,
          &obj[1].sx, &obj[1].sy, &obj[1].vx, &obj[1].vy, &obj[0].m, &obj[1].m);
    double MEi = 0, MEf = 0; //initial and final mechanical energy
    for (int i = 0; i < 2; i++) {
        //set initial linear and angular acceleration to 0
        obj[i].ax = 0, obj[i].ay = 0;
        obj[i].a = 0;
        obj[i].I = 0.4 * obj[i].m * r * r;
        //calculate initial mechanical energy
        MEi += 0.5 * obj[i].m * (pow(obj[i].vx, 2) + pow(obj[i].vy, 2));
    }

    //beginning time and delta t
    double t = 0, delta = 0.0001;
    double compx, compy;

    while (t <= 5) { //continue until simulation runs for 5s
        for (int i = 0; i < 2; i++) {
            //calculate new velocity and displacement values
            obj[i].vx += obj[i].ax * delta;
            obj[i].vy += obj[i].ay * delta;
            obj[i].sx += obj[i].vx * delta;
            obj[i].sy += obj[i].vy * delta;
            obj[i].w += obj[i].a * delta;
        }

        //if the two balls are in contact
        if (dist(obj[0].sx, obj[0].sy, obj[1].sx, obj[1].sy) < 2 * r) {
            //calculate distance between balls
            double comp = dist(obj[0].sx, obj[0].sy, obj[1].sx, obj[1].sy);
            compx = (obj[0].sx - obj[1].sx);
            compy = (obj[0].sy - obj[1].sy);

            //F = kx
            //force points along radial direction
            //force is broken into components and acceleration is calculated
            obj[0].ax = k * (r - comp / 2) * (compx / comp) / obj[0].m;
            obj[0].ay = k * (r - comp / 2) * (compy / comp) / obj[0].m;
            obj[1].ax = -k * (r - comp / 2) * (compx / comp) / obj[1].m;
            obj[1].ay = -k * (r - comp / 2) * (compy / comp) / obj[1].m;

            //calculating rotation
            double radialx, radialy; //unit radial vector
            radialx = (obj[1].sx - obj[0].sx) / comp;
            radialy = (obj[1].sy - obj[0].sy) / comp;
            double tangx, tangy; //unit tangential vector
            tangx = -radialy;
            tangy = radialx;
            double vxrel, vyrel; //relative velocities between balls
            vxrel = obj[1].vx - obj[0].vx;
            vyrel = obj[1].vy - obj[0].vy;

            //calculate direction of friction based on relative velocities and relative
            rotational velocites
            //at location of contact
            double frictionDir = vxrel*tangx + vyrel*tangy - obj[1].w * r - obj[0].w * r;

            //if there is no relative velocity, ignore rotation
            if (frictionDir == 0) {
                obj[0].a = 0;
                obj[1].a = 0;
                continue;
            }

            //compute direction of friction
            if (frictionDir < 0) {
                tangx *= -1;
                tangy *= -1;
            }

            //f = miu * k * x
            //break friction force into components
            double fricMag0 = abs(k * (r - comp / 2) * miu / obj[0].m);
            double fricMag1 = abs(k * (r - comp / 2) * miu / obj[1].m);
            obj[0].ax += fricMag0 * tangx;
            obj[0].ay += fricMag0 * tangy;
            obj[1].ax -= fricMag1 * tangx;
            obj[1].ay -= fricMag1 * tangy;

            //calculate torque, and angular acceleration
            if (frictionDir > 0) obj[0].a = r * abs(k * (r - comp / 2) * miu) / obj[0].I;
            else obj[0].a = -1 * r * abs(k * (r - comp / 2) * miu) / obj[0].I;
            obj[1].a = obj[0].a * obj[0].I / obj[1].I;

        } else { //acceleration of object stays 0
            for (int i = 0; i < 2; i++) {
                obj[i].ax = 0;
                obj[i].ay = 0;
                obj[i].a = 0;
            }
        }
        t += delta; //increment time
    }
    for (int i = 0; i < 2; i++) { //display final values
        printf("object %d\n", i + 1);
        printf("%.3lf %.3lf\n", obj[i].sx, obj[i].sy);
        printf("%.3lf %.3lf\n", obj[i].vx, obj[i].vy);
        printf("%.3lf %.3lf\n", obj[i].ax, obj[i].ay);
        printf("%.3lf\n", obj[i].w);
        printf("%.3lf\n", obj[i].a);

        //calculate final mechanical energy
        MEf += 0.5 * obj[i].m * (pow(obj[i].vx, 2) + pow(obj[i].vy, 2)) + 0.5 * obj[i].I * (pow(obj[i].w, 2));
    }
    //compare initial and final mechanical energy
    //unless I = 0, final mechanical energy should be lower
    printf("%.3lf %.3lf\n", MEi, MEf);
    return 0;
}