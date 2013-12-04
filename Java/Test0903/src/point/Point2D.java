/*
 * point2D
 *
 */

package point;
public class Point2D {
	private double x = 0;
	private double y = 0;
	public Point2D() {}
	public Point2D(double x, double y) {
		this.x = x; this.y = y;
	}
	public Point2D(Point2D p) { this(p.x, p.y); }
	public String toString() {
		return "("+x+","+y+")";
	}
	public static double distance (Point2D p1, Point2D p2) {
		return Math.sqrt( (p1.x - p2.x)*(p1.x - p2.x)  +
				          (p1.y - p2.y)*(p1.y - p2.y)  );
	}
}
