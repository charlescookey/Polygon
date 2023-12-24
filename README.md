# Polygon
 Given two polygons, it returns the union, intersection and difference between the two




To find if to points intersect
The equations of the lines are

p3   p2
 \  / 
  \/
  /\
 /  \
/    \
p1    p4

Pa = P1 + ua ( P2 - P1 )
Pb = P3 + ub ( P4 - P3 )

Solving for the point where Pa = Pb gives the following two equations in two unknowns (ua and ub)

x1 + ua (x2 - x1) = x3 + ub (x4 - x3)
y1 + ua (y2 - y1) = y3 + ub (y4 - y3)


ua = (x4-x3)(y1-y3) - (y4-y3)(x1-x3)
     -------------------------------
      (y4-y3)(x2-x1) - (x4-x3)(y2-y1)


ub = (x2-x1)(y1-y3) - (y2-y1)(x1-x3)
     -------------------------------
      (y4-y3)(x2-x1) - (x4-x3)(y2-y1)

Substituting either of these into the corresponding equation for the line gives the intersection point. For example the intersection point (x,y) is

x = x1 + ua (x2 - x1)
y = y1 + ua (y2 - y1)


To check if a point is inside the polygon


Consider a polygon made up of N vertices (xi,yi) where i ranges from 0 to N-1. The last vertex (xN,yN) is assumed to be the same as the first vertex (x0,y0), that is, the polygon is closed. To determine the status of a point (xp,yp) consider a horizontal ray emanating from (xp,yp) and to the right. If the number of times this ray intersects the line segments making up the polygon is even then the point is outside the polygon. Whereas if the number of intersections is odd then the point (xp,yp) lies inside the polygon. The following shows the ray for some sample points and should make the technique clear.

<img src="insidepoly.png"  />

Note: for the purposes of this discussion 0 will be considered even, the test for even or odd will be based on modulus 2, that is, if the number of intersections modulus 2 is 0 then the number is even, if it is 1 then it is odd.


To get the union

A = all vertices of Polygon A
B = all vertices of Polygon B

we find  all the intersection points, by running though all the edges of A 
then find if and where it intersects with B

IP = all points where A intersects with B

A* = All vertices in A that do not lie in B
B* = All vertices in B that do not lie in A

A! = All vertices in A that lie in B
B! = All vertices in B that lie in A

Union = A* + IP + B*
Intersect = A! + IP + B

DiffA = A* + IP + B!
DiffB = B* + IP + A!