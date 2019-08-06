import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.GraphicsEnvironment;
import java.awt.event.WindowEvent;
import java.awt.image.BufferedImage;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class Screen {
	
	private JFrame jframe;
	private JPanel jpanel;
	private JLabel upperLabel;
	public JLabel imageLabel;	// TODO
	private JLabel lowerLabel;
	private int height;
	private int width;
	
	// MAIN CONSTRUCTOR
	/**
	 * Screen Constructor
	 * @param width			Width of frame
	 * @param height		Height of frame
	 * @param posX			Up left point position of frame
	 * @param posY			Up left point position of frame
	 * @param exitOnClose	Terminates program if frame closes
	 */
	public Screen(int width, int height, int posX, int posY, boolean exitOnClose) {
		this.width = width;
		this.height = height;
		initGui(posX, posY, exitOnClose);
	}
	
	// SUB CONSTRUCTOR
	/**
	 * Screen Constructor. Centers the location on screen
	 * @param width			Width of frame
	 * @param height		Height of frame
	 * @param exitOnClose	Terminates program if frame closes
	 */
	public Screen(int width, int height, boolean exitOnClose) {
		this(width, 
				height, 
				GraphicsEnvironment.getLocalGraphicsEnvironment().getCenterPoint().x - width/2, 
				GraphicsEnvironment.getLocalGraphicsEnvironment().getCenterPoint().y - height/2, 
				exitOnClose);
	}
	
	private void initGui(int posX, int posY, boolean exitOnClose) {
		jframe = new JFrame("YapayZeka Odevi");
		jpanel = new JPanel(new BorderLayout()); 	
		upperLabel = new JLabel();
		imageLabel = new JLabel();
		lowerLabel = new JLabel();
		
		// make centered
		upperLabel.setHorizontalAlignment(JLabel.CENTER);
		imageLabel.setHorizontalAlignment(JLabel.CENTER);
		lowerLabel.setHorizontalAlignment(JLabel.CENTER);

		jpanel.add(upperLabel, BorderLayout.PAGE_START);
		jpanel.add(imageLabel, BorderLayout.CENTER);
		jpanel.add(lowerLabel, BorderLayout.PAGE_END);
		jframe.add(jpanel);
		jframe.setSize(width, height);
		jframe.setLocation(posX, posY);
		if (exitOnClose) {
			jframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		}
	}
	
	/**
	 * Fits and shows the image in frame, upper and lower captions still the same
	 * @param bi	Image on CENTER
	 */
	public void show(BufferedImage bi) {	
		jframe.setVisible(true);
		
		// TODO: BorderLayout'tan gercek boyutlari cekemedigim icin burada gecici cozum kullandim:
		int realWidth = jframe.getWidth() - upperLabel.getFont().getSize() - lowerLabel.getFont().getSize();
		int realHeight = jframe.getHeight() - upperLabel.getFont().getSize() - lowerLabel.getFont().getSize() - 120;
		
		BufferedImage newbi = ImageUtil.resizeImage(bi, realWidth, realHeight, true);
		imageLabel.setIcon(new ImageIcon(newbi));
		jframe.repaint();
	}
	
	/**
	 * Fits and shows the image in frame, updates upper and lower captions
	 * @param bi			Image on CENTER
	 * @param upperCaption	Text on PAGE_START
	 * @param lowerCaption	Text on PAGE_END
	 */
	public void show(BufferedImage bi, String upperCaption, String lowerCaption) {
		jframe.setVisible(true);
		show(bi);
		upperLabel.setText(upperCaption);
		lowerLabel.setText(lowerCaption);
	}
	
	public void close() {
		jframe.dispatchEvent(new WindowEvent(jframe, WindowEvent.WINDOW_CLOSING));
	}
	
	/********************************* SETTER/GETTER *********************************************/
	
	/**
	 * Sets upperText size
	 * @param size	New size of text
	 */
	public void setUpperTextSize(int size) {
		upperLabel.setFont(new Font(upperLabel.getFont().getFontName(), upperLabel.getFont().getStyle(), size));
	}
	
	/**
	 * Sets lowerText size
	 * @param size	New size of text
	 */
	public void setLowerTextSize(int size) {
		lowerLabel.setFont(new Font(lowerLabel.getFont().getFontName(), lowerLabel.getFont().getStyle(), size));
	}
	
	public int getUpperTextSize() {
		return upperLabel.getFont().getSize();
	}
	
	public int getLowerTextSize() {
		return lowerLabel.getFont().getSize();
	}
	
	/**
	 * Returns text size
	 * @return If text size differs with upper and lower, returns maximum size
	 */
	public int getTextSize() {
		return getUpperTextSize() > getLowerTextSize() ? getUpperTextSize() : getLowerTextSize();
	}
	
	/**
	 * Sets text size of both upperText and lowerText
	 * @param size	New size of text
	 */
	public void setTextSize(int size) {
		setUpperTextSize(size);
		setLowerTextSize(size);
	}
	
	/**
	 * Set size of the frame
	 * @param width		New frame size of width
	 * @param height	New frame size of height
	 */
	public void setSize(int width, int height) {
		this.width = width;
		this.height = height;
		jframe.setSize(height, height);
	}
	
	public int getHeight() {
		return height;
	}

	public int getWidth() {
		return width;
	}
	
	public void setTitle(String title) {
		jframe.setTitle(title);
	}
	
	public String getTitle() {
		return jframe.getTitle();
	}
	
	

}
