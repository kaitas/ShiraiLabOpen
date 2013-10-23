//拡張for文 p.196
public class Sample0402 {
	public static void main(String[] args) {
		double[] num = {1.5, 2.5, 3.5, 4.5, 5.5 };
		double sum = 0;
		for (double i:num) { //拡張for文=要素数だけ繰り返す
		    sum += i;
		}
		System.out.println("合計="+sum);

	}

}
