angle=23.26;
plate_height = 2.8;
servo = false;

module support() {
    difference () {
        union() {
            cube([20, 20, plate_height]);
            translate([20, 0, 0]) cube([50, 27, plate_height]);
//            translate([20, 50, 0]) cube([50, 35, plate_height]);
//            translate([20, 0, 0]) cube([15, 85, plate_height]);
//            translate([0, 65, 0]) cube([20, 20, plate_height]);
            translate([30, 0, 0]) cube([40, 27, 20]);
            translate([37, 17, 0]) rotate(a = [0, -angle, 0])
            translate([0, 0, 20]) rotate(a = [-90, 0, 0])
                translate([0, 0, 0]) cube([54, 25, 10]);
        }
        translate([-5, -5, -10]) cube([60, 60, 10]);
        translate([0, -5, plate_height]) cube([30, 60, 60]);
        translate([0, -1, plate_height]) cube([43.5, 18, 30]);
    }
}

module servo(extra_bottom = false, extra_left = false) {
    translate([37, 0, 0]) rotate(a = [0, -angle, 0])
    translate([0, 0, 20]) rotate(a = [-90, 0, 0])
    color("red") union() {
        translate([7, 0, 0]) cube([40, 20, 38]);
        if (extra_left && extra_bottom) {
            translate([7, -5, -5]) cube([40, 25, 38]);
        } else {
            if (extra_bottom) {
                translate([7, 0, -5]) cube([40, 20, 10]);
            }
            if (extra_left) {
                translate([7, -5, 0]) cube([40, 20, 38]);
            }
        }
        translate([0, 0, 27]) difference () {
            cube([54, 20, 2.5]);
            translate([3, 5, -1]) cylinder(h = 4, r = 2);
            translate([3, 15, -1]) cylinder(h = 4, r = 2);
            translate([51, 5, -1]) cylinder(h = 4, r = 2);
            translate([51, 15, -1]) cylinder(h = 4, r = 2);
        }
        translate([37.5, 10, 0]) cylinder(h = 46, r = 4.5);
        translate([37.5, 10, 46]) difference() {
            cylinder(h = 2, r = 24);
            rotate(a = [0, 0, angle]) translate([-24, -24, -1]) cube([ 48, 24, 4 ]);
        }
    }
}

if (servo) {
    support();
    servo();
} else {
    difference() {
        support();
        servo(true, true);
    }
}
