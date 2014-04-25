# Element( flags description blank blank mark_x mark_y text_x text_y text_direction text_scale text_flags
# Pin( x y thickness clearance mask drillhole name number flags
# ElementLine( x1 y1 x2 y2 thickness)
# ElementArc( centre_x centre_y radius_x radius_y startAngle deltaAngle thickness)  startAngle in degs, 0=East, +ve deltaAngle anti-clockwise
# Measurements are taken from the datasheet
#
Element( 0x00 "Single Large Terminal Pad" "" "" 0 0 150 100 3 100 0x00)
(
  Pin(   0     0  200  70   205  94 "a"  "1" 0x000)
)
