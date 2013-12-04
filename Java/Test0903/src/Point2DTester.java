/*
 * Point2DTester
 * */

import point.Point2D;

public class Point2DTester {
	public static void main(String[] args) {
		Point2D p1 = new Point2D();
		Point2D p2 = new Point2D(3.0f, 4.0f);
		System.out.println("p1 = "+ p1);
		System.out.println("p2 = "+ p2);
		System.out.println("distance = "+ Point2D.distance(p1,p2));
	}

}
