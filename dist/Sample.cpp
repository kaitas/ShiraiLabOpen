//http://sai-lab.net/blog/?p=129
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include "GL/glut.h"
#include "DistortionCorrection.h"

//グローバル変数
DistortionCorrection g_DC;
float g_cx = 0;
float g_cy = 0;
float g_kR = 0;
float g_kG = 0;
float g_kB = 0;

static void display(void)
{
	// 画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//モデルビュー変換行列の設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//ぐるぐる回るティーポット描画
	static float r = 0;
	glRotatef(r, 0.0, 1.0, 0.0);
	glutSolidTeapot(1);
	r += 1.0;

	//歪み補正実行
	g_DC.runDC();

	// ダブルバッファリング
	glutSwapBuffers();
}

static void reshape(int w, int h)
{
	//ウィンドウ全体をビューポートにする
	glViewport(0, 0, w, h);
  
	//透視変換行列の指定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (double)w / (double)h, 0.1, 100.0);
	gluLookAt(0.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//歪み補正パラメータ設定
	g_cx = w/2.0f;
	g_cy = h/2.0f;
	g_DC.setCenter(g_cx, g_cy);
}

static void idle(void)
{
	glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	
	// ESCで終了
	case '\033':
		exit(0);
		break;

	//RGBそれぞれの歪み系数設定
	case 'R':
		g_kR += 0.0001f;
		g_DC.setParams(g_kR, g_kG, g_kB);
		break;
	case 'r':
		g_kR -= 0.0001f;
		g_DC.setParams(g_kR, g_kG, g_kB);
		break;
	case 'G':
		g_kG += 0.0001f;
		g_DC.setParams(g_kR, g_kG, g_kB);
		break;
	case 'g':
		g_kG -= 0.0001f;
		g_DC.setParams(g_kR, g_kG, g_kB);
		break;
	case 'B':
		g_kB += 0.0001f;
		g_DC.setParams(g_kR, g_kG, g_kB);
		break;
	case 'b':
		g_kB -= 0.0001f;
		g_DC.setParams(g_kR, g_kG, g_kB);
		break;

	//デバッグ表示切り替え
	case 'D':
	case 'd':
		g_DC.switchDebugMode();
		break;
	}
}

static void special(int key, int x, int y)
{
	switch(key){
	case GLUT_KEY_UP:
		g_cy += 1.0f;
		break;
	case GLUT_KEY_DOWN:
		g_cy -= 1.0f;
		break;
	case GLUT_KEY_RIGHT:
		g_cx += 1.0f;
		break;
	case GLUT_KEY_LEFT:
		g_cx -= 1.0f;
		break;
	}

	//歪みの中心を設定
	g_DC.setCenter(g_cx, g_cy);
}

void initGL(int argc, char *argv[])
{
	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Distortion Correction Sample");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glClearColor(0.0, 0.0, 0.0, 0.0); //背景色※必ず黒にすること
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[])
{
	//glutの初期化
	initGL(argc, argv);
	
	//歪み補正機能初期化
	g_DC.init();

	glutMainLoop();
	return 0;
}