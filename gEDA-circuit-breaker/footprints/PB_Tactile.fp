# Element( flags description blank blank mark_x mark_y text_x text_y text_direction text_scale text_flags
# Pin( x y thickness clearance mask drillhole name number flags
# ElementLine( x1 y1 x2 y2 thickness)
# ElementArc( centre_x centre_y radius_x radius_y startAngle deltaAngle thickness)  startAngle in degs, 0=East, +ve deltaAngle anti-clockwise
# Measurements are taken from the datasheet
#
Element( 0x00 "Tactile pushbutton" "" "" 0 0 150 100 3 100 0x00)
(
  Pin(     0   0  60  70   70  38 "a"  "1" 0x101)
  Pin(   200   0  60  70   70  38 "b"  "2" 0x001)
  Pin(   200 200  60  70   70  38 "c"  "3" 0x001)
  Pin(     0 200  60  70   70  38 "d"  "4" 0x001)
  ElementLine(   0  50   0 150 10)  # left, starting top-left going anti-clockwise
  ElementLine( 200  50 200 150 10)  # right
  ElementLine(  50 200 150 200 10)  # top
  ElementLine(  50   0 150   0 10)  # bottom
  ElementArc ( 100 100 50 50 0 360 10)
)
