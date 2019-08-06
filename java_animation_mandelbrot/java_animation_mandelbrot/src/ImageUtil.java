import java.awt.Graphics;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class ImageUtil {

	/**
	 * Deep copies the source image
	 * @param source	Source image
	 * @return	Deep copied image
	 */
	public static BufferedImage copyImage(BufferedImage source){
	    BufferedImage b = new BufferedImage(source.getWidth(), source.getHeight(), source.getType());
	    Graphics g = b.getGraphics();
	    g.drawImage(source, 0, 0, null);
	    g.dispose();
	    return b;
	}
	
	/**
	 * Resizes the image
	 * @param image			Source image
	 * @param newX			New width or x of image
	 * @param newY			New height or y of image
	 * @param keepRatio		If true, scale_x will be equal to scale_y
	 * @return	Scaled image
	 */
	public static BufferedImage resizeImage(BufferedImage image, int newX, int newY, boolean keepRatio) {	
		double scaleX = (double)newX / image.getWidth();
		double scaleY = (double)newY / image.getHeight();
		
		if (keepRatio) {
			if (scaleX > scaleY) {
				scaleX = scaleY;
			}
			else
			{
				scaleY = scaleX;
			}
		}
		
    	AffineTransform at = new AffineTransform();
    	at.scale(scaleX, scaleY);
    	AffineTransformOp scaleOp = 
    	   new AffineTransformOp(at, AffineTransformOp.TYPE_BILINEAR);
    	return  scaleOp.filter(image, null);
	}
	
    /**
     * Reads image from file
     * @param filename	File location (ex: "C:\\Users\\UserName\\Desktop\\test.png")
     * @return	Imported image as BufferedImage
     */
    public static BufferedImage loadPNG(String filename) {
    	BufferedImage map = null;
    	try {
    		map = ImageIO.read(new File(filename));
    	} catch (IOException e) {
    		System.err.println(filename + " file not found!");
    	}
    	return map;
    }
	
}
