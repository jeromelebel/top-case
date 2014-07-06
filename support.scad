angle=23.26;
plate_height = 2.56;
plate_width = 27;
plate_servo_holder_thickness = 10;
servo = false;
simple = false;
$fn = 50;
version = "right";

servo_length = 41;
servo_main_part_height = 27;

module support(version) {
    difference () {
        union() {
            cube([70, plate_width, plate_height]);
            if (simple) {
                translate([30, plate_width - plate_servo_holder_thickness, 0]) cube([40, 10, 20]);
            } else {
                translate([30, 0, 0]) cube([40, plate_width, 20]);
            }
            translate([37, (version == "left")?(plate_width - plate_servo_holder_thickness):0, 0]) rotate(a = [0, -angle, 0])
            translate([0, 0, 20]) rotate(a = [-90, 0, 0])
                translate([0, 0, 0]) cube([54, 25, plate_servo_holder_thickness]);
        }
        translate([-5, -5, -10]) cube([60, 60, 10]);
        translate([0, -5, plate_height]) cube([30, 60, 60]);
        if (version == "left") {
            translate([-1, 20, -1]) cube([21, plate_width - 20 + 1, plate_height + 2]);
        }
        translate([0, (version == "left")?-1:plate_servo_holder_thickness, plate_height]) cube([43, plate_width - plate_servo_holder_thickness + 1, 30]);
        translate([15, (version == "left")?10:17, -1]) cylinder(h = plate_height + 2, r = 0.5);
        translate([60, (version == "left")?10:17, -1]) cylinder(h = plate_height + 20, r = 0.5);
        translate([60, (version == "left")?10:17, plate_height]) cylinder(h = 20, r = 6);
    }
}

module servo(extra_bottom = false, extra_left = false, extra_right = false, cylinder_for_holes = false) {
    color("red") union() {
        translate([servo_main_part_height - (servo_length / 2), 0, 0]) cube([servo_length, 20, 38]);
        if (extra_left && extra_right && extra_bottom) {
            translate([servo_main_part_height - (servo_length / 2), -5, -5]) cube([servo_length, 30, 43]);
        } else if (extra_left && extra_bottom) {
            translate([servo_main_part_height - (servo_length / 2), -5, -5]) cube([servo_length, 25, 43]);
        } else if (extra_right && extra_bottom) {
            translate([servo_main_part_height - (servo_length / 2), 0, -5]) cube([servo_length, 25, 43]);
        } else if (extra_right && extra_left) {
            translate([servo_main_part_height - (servo_length / 2), -5, -5]) cube([servo_length, 25, 38]);
        } else if (extra_bottom) {
                translate([servo_main_part_height - (servo_length / 2), 0, -5]) cube([servo_length, 20, 10]);
        } else if (extra_left) {
            translate([servo_main_part_height - (servo_length / 2), -5, 0]) cube([servo_length, 20, 38]);
        } else if (extra_right) {
            translate([servo_main_part_height - (servo_length / 2), 5, 0]) cube([servo_length, 20, 38]);
        }
        if (cylinder_for_holes) {
            translate([0, 0, servo_main_part_height]) union () {
                cube([54, 20, 2.5]);
                translate([3, 5, -11]) cylinder(h = 24, r = 2);
                translate([3, 15, -11]) cylinder(h = 24, r = 2);
                translate([51, 5, -11]) cylinder(h = 24, r = 2);
                translate([51, 15, -11]) cylinder(h = 24, r = 2);
            }
        } else {
            translate([0, 0, servo_main_part_height]) difference () {
                cube([54, 20, 2.5]);
                translate([3, 5, -1]) cylinder(h = 4, r = 2);
                translate([3, 15, -1]) cylinder(h = 4, r = 2);
                translate([51, 5, -1]) cylinder(h = 4, r = 2);
                translate([51, 15, -1]) cylinder(h = 4, r = 2);
            }
        }
        translate([37.5, 10, 0]) cylinder(h = 46, r = 4.5);
        translate([37.5, 10, 46]) difference() {
            cylinder(h = 2, r = 24);
            rotate(a = [0, 0, angle]) translate([-24, -24, -1]) cube([ 48, 24, 4 ]);
        }
    }
}

rotate([ (version == "left") ?-90:90, 0, 0 ])
if (servo) {
    support(version);
    translate([37, 0, 0]) rotate(a = [0, -angle, 0])
    translate([0, (version == "left")?0:servo_main_part_height, (version == "left")?20:0]) rotate(a = [(version == "left")?-90:90, 0, 0])
    servo();
} else {
    difference() {
        support(version);
        translate([37, 0, 0]) rotate(a = [0, -angle, 0])
        translate([0, (version == "left")?0:servo_main_part_height, (version == "left")?20:0]) rotate(a = [(version == "left")?-90:90, 0, 0])
        servo(true, version == "left", version == "right", true);
    }
}