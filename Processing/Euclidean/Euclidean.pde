// Draw Euclidean from "Programming Collective Intelligence"
// http://shop.oreilly.com/product/9780596529321.do
// Akihiko SHIRAI (shirai@mail.com) 20140702
// for Master Lecture "Entainment Systems"
// it may include some errors 

//Libraries
import processing.pdf.*;

// globals
  PGraphicsPDF pdf;
  float L =5; // levels
  int MOVS = 7; // num of movies (1-7)
  int PERS = 7; // num of person (0-6)
  Table table;
  float[][] Critics = new float[7][7];
  String[] Names = new String[7];

//convert to screen coordinates
  float XChange(float x){ 
    x = width*0.1+100*x;
    return x;
  }
   float YChange(float y){ 
    y = height*(1-0.1)-100*y;
    return y;
  }

// To get n! (fact)
  int fact(int n) {
    if (n == 0) {
      return 1;
    } else {
      return n * fact(n - 1);
    }
  }

  void LoadDataFile(){
    table = loadTable("data.csv","header");
    for (TableRow row : table.rows()) {
      int id = row.getInt("id");
      Critics[id][0] = id;
      Names[id] = row.getString("Name");
      for (int mov = 1; mov<=6 ; mov++ ) {
        Critics[id][mov] = row.getFloat("M"+mov);
      }
      print(Names[id] + "(id=" + Critics[id][0] + ")");
      for(int i=1;i<PERS;i++){
        print(" " + Critics[id][i]);
      }
      println("");
    }
  }

//line
  void DrawAxis(int m1, int m2){
    float M =0.1;
    background(255);
    fill(0,0,0);
    strokeWeight(3);
    line(width*M,0,width*M,height);//tate_sen
    line(0,height*(1-M),width,height*(1-M));//yoko_sen
    text("M"+m1,width*(M/2)*0.3,height*(M/2));
    text("M"+m2,width*(1-M)+20,height*(1-M/2));
    for(int i=0;i<L;i++){
      float d1=width*M+i*(width*0.8)/L;
      float d2=height*M+(i+1)*(height*0.8)/L;
      line(width*M*0.8,d1,width*M,d1);
      line(d2,height*(1-M),d2,height*(1-M*0.8));
      text(5-i,width*M*0.3,d1+7);//tate_sen_kazu
      text(i+1,d2-7,height*(1-M*0.3),d2);//yoko_sen_kazu
    } 
  }

  void DrawName(int id,int Mov1,int Mov2){
    fill(0,0,100);
    text("*"+Names[id]+"("+Critics[id][Mov1]+","+Critics[id][Mov2]+")",
    XChange(Critics[id][Mov1]), YChange(Critics[id][Mov2])+10);
  }

  void Draw2Movies(int Mov1, int Mov2){
    for (int i=0; i<Names.length; i++ ) {
      DrawName(i,Mov1,Mov2);
    }
  }

// you can also use dist(x1, y1, x2, y2) from Processing API
float Distance(int m1,int m2,int p1, int p2) {
    return sqrt(
      (Critics[p2][m1] - Critics[p1][m1])*(Critics[p2][m1] - Critics[p1][m1])
      +(Critics[p2][m2] - Critics[p1][m2])*(Critics[p2][m2] - Critics[p1][m2])
    ); 
}

  void DrawDistance(float d, int m1,int m2,int p1, int p2){
   float x,y;
    x = ( Critics[p1][m1] + Critics[p2][m1] ) / 2.0f ;
    y = ( Critics[p1][m2] + Critics[p2][m2] ) / 2.0f ;
    fill(255,0,0,50);  
    text(""+(round(d*100)/100.0f),XChange(x)-10,YChange(y));
  }

  void DrawEuclidean(int m1,int m2,int p1, int p2){
    // println(map(Distance(m1,m2,p1,p2),0,5,50,0)); //How to know map()
    stroke(10,10,10,map(Distance(m1,m2,p1,p2),0,5,50,10));
    strokeWeight(0.5);
    line(XChange(Critics[p1][m1]),YChange(Critics[p1][m2]),
         XChange(Critics[p2][m1]),YChange(Critics[p2][m2]));
  }

  void setup() {  
    size(640,640);
    //Open new PDF file
    pdf = (PGraphicsPDF)beginRecord(PDF, "Eucledean.pdf");
    beginRecord(pdf);

    textSize(15);
    fill(90,90,90);//line_color
    LoadDataFile();
    FloatDict GlobalDist = new FloatDict();
    for (int m1=1; m1<MOVS; m1++) {
      for (int m2=m1+1; m2<MOVS; m2++) {
        println("----Movies(M"+m1+",M"+m2+")-----");
        DrawAxis(m1,m2);
        Draw2Movies(m1,m2);
        FloatDict Dist = new FloatDict(); // for each graph
        // int Combination = fact(7) / ( fact(2) * fact( PERS - 2 ) );
        // println("Combination("+PERS+",2)="+Combination);
        for (int p1=0; p1<PERS-1; p1++) {
          for (int p2=p1+1; p2<=PERS-1; p2++) {
            float d = Distance(m1,m2,p1,p2);
            DrawEuclidean(m1,m2,p1,p2);
            DrawDistance(d, m1,m2,p1,p2); // draw distance on the line
            GlobalDist.set("M"+m1+",M"+m2+","+p1+","+p2, d );
            Dist.set(""+p1+","+p2, Distance(m1,m2,p1,p2));
          }
         }
        Dist.sortValues(); //println(Dist);
        float[] DistVal = Dist.valueArray();
        String[] DistPers = Dist.keyArray();
        fill(255,10,10);
        text("Eucledean[ M"+m1+" - M"+m2+" ]", width*0.6, height*0.05);
        text("min ["+DistPers[0]+"]="+DistVal[0], width*0.6, height*0.1);
        text("Max ["+DistPers[DistPers.length-1]+"]="+DistVal[DistVal.length-1], width*0.6, height*0.15);
        save("png/M"+m1+"-M"+m2+".png");

        //save a PDF page
        pdf.nextPage();

      }
    }
    GlobalDist.sortValues();
    println(GlobalDist);
    //close PDF file
    endRecord();
    exit();  // Quit
  }

