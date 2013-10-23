// 2次元配列 p.208～
public class Sample0403 {
	public static void main(String[] args) {
		int[][] kuku = new int [9][9];
		for (int i=0; i<9; i++) {
			for (int j=0; j<9; j++) {
				kuku[i][j] = (i+1) * (j+1);
				//配列の要素はゼロから始まるため、算術との整合で+1必要
			}
		}
		for (int i=0; i<9; i++) {
		     System.out.println((i+1)+"の段");
		     for (int j=0; j<9; j++) {
		    	 // "1桁目を右ぞろえして"という仕様から
		    	 System.out.printf("%3d", kuku[i][j]);
		     }
		     System.out.println(); //空改行
		}
	}

}
