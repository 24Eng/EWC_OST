sheetThickness = 3;
boltDiameter = 6.34;
thickness = 5;
sheetOffsetFromBoltCenter = 10.60660172;
roundingRadius = 1;


boltRadius = boltDiameter/2;
cubeWidth = (thickness*2) + (boltRadius) + (sheetThickness) + (sheetOffsetFromBoltCenter)-roundingRadius;
cubeLength = cubeWidth;
cubeDepth = (thickness*3)-roundingRadius;
boltOffsetFromOrigin = sheetOffsetFromBoltCenter + thickness + sheetThickness;
$fn=30;
$vpr=[50,0,30+$t*360];

difference(){
//    translate([roundingRadius,roundingRadius,roundingRadius]){
    translate([0,0,0]){
        rotate([0,0,0]){
            minkowski(){
                cube([cubeWidth, cubeLength, cubeDepth]);
                sphere(roundingRadius);
            }
        }
    }
    translate([thickness,thickness+sheetThickness,thickness]){
        rotate([0,0,0]){
            cube([sheetThickness, cubeLength*2, cubeDepth*2]);
        }
    }
    translate([thickness+sheetThickness,thickness+sheetThickness,thickness]){
        rotate([0,0,270]){
            cube([sheetThickness, cubeLength*2, cubeDepth*2]);
        }
    }
    
    translate([boltOffsetFromOrigin,boltOffsetFromOrigin,0]){
        rotate([0,0,0]){
            cylinder(cubeDepth*3, boltRadius, boltRadius, center=true);
        }
    }
}