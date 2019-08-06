import java.awt.Color;
import java.awt.image.BufferedImage;

public class Main {
	public static void main(String[] args) {

		int width = 1280, height = 720;
		int init_iter = 4;
		double bound = 2.0;
		double zoom = 0.93;
		double xpos = 0.387972;
		double ypos = 0.261575;
		int max_iter = 200;
		
		Screen s = new Screen(width, height, true);
		s.setTitle("Mandelbrot");
		s.setTextSize(20);
		
		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_3BYTE_BGR);
		
		double z = 4.0;
		int iter = init_iter;
		for (int i=0; i<=max_iter; i+=1) {
			iter+=1;
			z *= zoom;
	        for (int row = 0; row < height; row++) {
	            for (int col = 0; col < width; col++) {
	                double c_re = z*(col - width/2)/width*4 + xpos;
	                double c_im = z*(row - height/2)/width*4 + ypos;
	                double x = 0, y = 0;
	                int iterations = 0;
	                while (x*x+y*y < bound*bound && iterations < iter) {
	                    double x_new = x*x-y*y+c_re;
	                    y = 2*x*y+c_im;
	                    x = x_new;
	                    iterations++;
	                } 
	                // color!
	                float c[] = Color.RGBtoHSB(iterations*5, iterations*7, iterations*11, null);
	                if (iterations < iter) image.setRGB(col, row, Color.HSBtoRGB(c[0], c[1], c[2]));
	                else image.setRGB(col, row, iterations);
	            }
	        }	
	        s.show(image, "iteration: " + i + "/" + max_iter, "github.com/salihmarangoz" );
		}
	}
}
