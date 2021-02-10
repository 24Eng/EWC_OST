// 2020-12-23 Countersunk foot

boltDiameter = 4.65;
boltHeadDiameter = 9.5;
boltHeadThickness = 3.5;
footDiameter = 22;
footThickness = 5;


boltRadius = boltDiameter/2;
boltHeadRadius = boltHeadDiameter/2;
footRadius = footDiameter/2;

$fn=30;
$vpr= [150,$t*360,0];

difference(){
    translate([0,0,0]){
        cylinder(footThickness*2, footRadius, boltHeadRadius);
        cylinder(footThickness, footRadius, footRadius);
    }
    
    translate([0,0,-0.01]){
        cylinder(footThickness*5, boltRadius, boltRadius, center = true);
        cylinder(boltHeadThickness, boltHeadRadius, boltHeadRadius);
    }
}