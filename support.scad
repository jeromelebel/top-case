angle=23.26;

union() {
    cube([20, 20, 10]);
    translate([20, 0, 0]) cube([50, 35, 10]);
    translate([20, 35, 0]) cube([15, 50, 10]);
    translate([0, 65, 0]) cube([20, 20, 10]);
}

translate([37, 0, 0]) rotate(a = [0, -angle, 0])
translate([0, 0, 20]) rotate(a = [-90, 0, 0])
color("red") union() {
    translate([7, 0, 0]) cube([40, 20, 38]);
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