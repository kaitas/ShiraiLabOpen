import processing.pdf.*;

PGraphicsPDF pdf;

PImage[] img = new PImage[12];

void setup() {
  size(1280, 960);
  for (int i =0; i<img.length; i++) {
    img[i] = loadImage("png/"+i+".png");
  }
  pdf = (PGraphicsPDF)beginRecord(PDF, "MyBook.pdf");
  beginRecord(pdf);
}

void draw() {
  image(img[frameCount], 0, 0, width, height);
  stroke(0,0,0);
  text(" - " + frameCount + " - ", width/2, height*0.9);
  if(frameCount == img.length-1) {
    endRecord();
    exit();  // Quit
  } else {
    pdf.nextPage();
  }
}
