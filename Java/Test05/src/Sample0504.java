// 0504
class testClass {
	String name;  int number;
}
public class Sample0504 {
	public static void main(String[] args) {
		testClass data1 = new testClass();
		testClass data2 = new testClass();
		data1.name = "足立幸一"; data1.number = 1000;
		data2.name = "仲田真二"; data1.number = 5555;
		System.out.println(data1.number + ":"+ data1.name);
		System.out.println(data2.number + ":"+ data2.name);
	}

}
