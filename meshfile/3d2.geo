lc = 0.25;
//+
Point(1) = {0, 0, 0, lc};
//+
Point(2) = {1, 0, 0, lc};
//+
Point(3) = {1, 1, 0, lc};
//+
Point(4) = {0, 1, 0, lc};
//+
Point(5) = {0, 0, 1, lc};
//+
Point(6) = {1, 0, 1, lc};

Point(7) = {1, 1, 1, lc};

Point(8) = {0, 1, 1, lc};
//+
Line(1) = {1, 2};
//+
Line(2) = {2, 3};
//+
Line(3) = {3, 4};
//+
Line(4) = {4, 1};
//+
Line(5) = {6, 7};
//+
Line(6) = {7, 8};
//+
Line(7) = {8, 5};
//+
Line(8) = {5, 6};
//+
Line(9) = {2, 6};
//+
Line(10) = {3, 7};
//+
Line(11) = {4, 8};
//+
Line(12) = {1, 5};
//+

//+
Line Loop(13) = {1, 2, 3, 4};
//+
Plane Surface(14) = {13};
//+
Line Loop(15) = {3, 11, -6, -10};
//+
Plane Surface(16) = {15};
//+
Line Loop(17) = {4, 12, -7, -11};
//+
Plane Surface(18) = {17};
//+
Line Loop(19) = {1, 9, -8, -12};
//+
Plane Surface(20) = {19};
//+
Line Loop(21) = {2, 10, -5, -9};
//+
Plane Surface(22) = {21};
//+
Line Loop(23) = {8, 5, 6, 7};
//+
Plane Surface(24) = {23};
//+
Surface Loop(25) = {20, 14, 22, 16, 18, 24};
//+
Volume(26) = {25};

//+
Physical Surface("1", 1) = {20, 22, 16, 18, 24, 14};

//+
Physical Volume("1",3) = {26};
//+
Coherence;
