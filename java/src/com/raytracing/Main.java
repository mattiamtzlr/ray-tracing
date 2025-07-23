package com.raytracing;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.time.Duration;
import java.time.Instant;
import java.util.Scanner;

public class Main {

    // dev boolean => set to false when rendering high quality
    private static final boolean DEV = true;

    // output image format, one of the values of the following enum:
    private enum FileType {PPM, BMP}

    private static final FileType OUTPUT_FILE_TYPE = FileType.BMP;

    public static void main(String[] args) {
        Instant start = Instant.now();

        Scanner bob = new Scanner(System.in);

        // image properties
        double aspectRatio = (double) 16 / 9;

        int imageWidth;
        if (DEV) {
            imageWidth = 500;
        } else {
            System.out.print("Image Width: ");
            imageWidth = bob.nextInt();
        }

        int samplesPerPixel;
        if (DEV) {
            samplesPerPixel = 10;
        } else {
            System.out.print("Samples per Pixel: ");
            samplesPerPixel = bob.nextInt();
        }

        final int maxDepth = 100;

        // world
        HittableList world = new HittableList();

        // camera
        Point3 lookFrom;
        Point3 lookAt;
        int vFOV;
        double aperture = 0;
        Color background = Utility.hexToColor("#bce5f5");

        final int scene = 5;
        switch (scene) {
            case 1: // --------------------------------------------------------- small Spheres
                world.add(Scenes.smallSpheres());
                lookFrom = new Point3(13, 2.2, 4);
                lookAt = new Point3(0, 0.4, 0);
                vFOV = 20;
                break;

            case 2: // --------------------------------------------------------- checkered Spheres
                world.add(Scenes.checkeredSpheres());
                lookFrom = new Point3(0, 0, 20);
                lookAt = new Point3(0, 0, 0);
                vFOV = 30;
                break;

            case 3: // --------------------------------------------------------- perlin Spheres
                world.add(Scenes.perlinSpheres());
                lookFrom = new Point3(4, 4, 10);
                lookAt = new Point3(0, 1.8, -2.5);
                vFOV = 40;
                break;

            case 4: // --------------------------------------------------------- earth & moon
                world.add(Scenes.earth());
                lookFrom = new Point3(5, 2, -1);
                lookAt = new Point3(0, 0, -1.5);
                vFOV = 60;
                break;

            case 5: // --------------------------------------------------------- lights
                if (DEV) samplesPerPixel = 75;
                background.set(new Color());

                world.add(Scenes.lights());
                lookFrom = new Point3(20, 4, 5);
                lookAt = new Point3(0, 2, -.7);
                vFOV = 20;
                break;

            case 6: // --------------------------------------------------------- cornell box
                if (DEV) samplesPerPixel = 30;
                background.set(new Color());
                aspectRatio = 1;

                world.add(Scenes.cornellBox());
                lookFrom = new Point3(250, 250, 1200);
                lookAt = new Point3(250, 250, 0);
                vFOV = 40;
                break;

            case 7: // --------------------------------------------------------- cornell box smoke
                if (DEV) samplesPerPixel = 75;
                background.set(new Color());
                aspectRatio = 1;

                world.add(Scenes.cornellBoxSmoke());
                lookFrom = new Point3(250, 250, 1200);
                lookAt = new Point3(250, 250, 0);
                vFOV = 40;
                break;

            case 8: // --------------------------------------------------------- final scene for book 2
                if (DEV) {
                    samplesPerPixel = 15;
                    imageWidth = 600;
                }
                background.set(new Color());

                world.add(Scenes.finalScene());
                lookFrom = new Point3(700, 160, 900);
                lookAt = new Point3(150, 140, 0);
                vFOV = 50;
                aperture = .7;
                break;

            case 20: // --------------------------------------------------------- spheres inside box
                if (DEV) samplesPerPixel = 100;
                background.set(new Color());

                world.add(Scenes.insideBox());
                lookFrom = new Point3(8, 1.8, 2.5);
                lookAt = new Point3(0, 0.4, 0);
                vFOV = 45;
                aperture = 10;
                break;

            case 21: // --------------------------------------------------------- all rotations
                if (DEV) samplesPerPixel = 75;
                background.set(new Color());

                world.add(Scenes.rotations());
                lookFrom = new Point3(0, 5.8, 10);
                lookAt = new Point3(0, 3.5, 0);
                vFOV = 60;
                break;

            case 22: // --------------------------------------------------------- bokeh
                if (DEV) samplesPerPixel = 75;
                background.set(new Color());

                world.add(Scenes.bokeh());
                lookFrom = new Point3(0, 1.5, 40);
                lookAt = new Point3(0, 1.3, 35);

                vFOV = 40;
                aperture = 0.3;
                break;


            default: // --------------------------------------------------------- default scene
                world.add(Scenes.standardScene());
                lookFrom = new Point3(13, 2.2, 4);
                lookAt = new Point3(0, 0.4, 0);
                vFOV = 20;
                aperture = 0.1;
                break;
        }

        final int imageHeight = (int) (imageWidth / aspectRatio);

        Vec3 viewUp = new Vec3(0, 1, 0); // horizontally level view
        double distToFocus = Vec3.sub(lookFrom, lookAt).length();

        Camera cam = new Camera(
            lookFrom, lookAt, viewUp, vFOV, aspectRatio, aperture, distToFocus, 0, 1
        );

        String fileName = switch (OUTPUT_FILE_TYPE) {
            case PPM -> "output.ppm";
            case BMP -> "output.bmp";
        };

        try (FileWriter writer = new FileWriter(fileName)) {

            switch (OUTPUT_FILE_TYPE) {
                case PPM -> {
                    StringBuilder output = new StringBuilder();
                    output.append(String.format("P3\n%d %d\n255\n", imageWidth, imageHeight)); // header of file

                    for (int j = imageHeight - 1; j >= 0; j--) {
                        // progress
                        System.out.print("\rScanlines remaining: " + j + " of " + imageHeight + " --- " +
                            (int) (((double) (imageHeight - j) / imageHeight) * 100) + "% completed");
                        System.out.flush();

                        for (int i = 0; i < imageWidth; i++) {
                            Color pixelColor = new Color(0, 0, 0);
                            // multiple samples per pixel
                            for (int s = 0; s < samplesPerPixel; s++) {
                                double u = (i + Utility.randomDouble()) / (imageWidth - 1);
                                double v = (j + Utility.randomDouble()) / (imageHeight - 1);
                                Ray r = cam.getRay(u, v);
                                pixelColor = Vec3.add(pixelColor, rayColor(r, background, world, maxDepth)).toColor();
                            }
                            output.append(writeColor(pixelColor, samplesPerPixel));
                        }
                    }

                    writer.write(output.toString());
                }

                case BMP -> {
                    BufferedImage image = new BufferedImage(imageWidth, imageHeight, BufferedImage.TYPE_INT_RGB);

                    for (int y = imageHeight - 1; y >= 0; y--) {
                        // progress
                        System.out.print("\rScanlines remaining: " + y + " of " + imageHeight + " --- " +
                            (int) (((double) (imageHeight - y) / imageHeight) * 100) + "% completed");
                        System.out.flush();

                        for (int x = 0; x < imageWidth; x++) {
                            Color pixelColor = new Color(0, 0, 0);

                            // multiple samples per pixel
                            for (int s = 0; s < samplesPerPixel; s++) {
                                double u = (x + Utility.randomDouble()) / (imageWidth - 1);
                                double v = (y + Utility.randomDouble()) / (imageHeight - 1);
                                Ray r = cam.getRay(u, v);
                                pixelColor = Vec3.add(pixelColor, rayColor(r, background, world, maxDepth)).toColor();
                            }

                            image.setRGB(x, imageHeight - (y + 1), writeColorBMP(pixelColor,
                                samplesPerPixel));
                        }
                    }

                    File output = new File(fileName);
                    ImageIO.write(image, "bmp", output);
                }
            }

            Instant finish = Instant.now();
            double timeElapsed = Duration.between(start, finish).toMillis();
            String timeScale = "milliseconds";

            if (timeElapsed > 1000) {
                timeElapsed = timeElapsed / 1000;
                timeScale = "seconds";
            }
            if (timeElapsed > 60 && timeScale.equals("seconds")) {
                timeElapsed = timeElapsed / 60;
                timeScale = "minutes";
            }
            if (timeElapsed > 60 && timeScale.equals("minutes")) {
                timeElapsed = timeElapsed / 60;
                timeScale = "hours";
            }

            System.out.printf("\nSuccessfully wrote to '%s' in %.2f %s.", fileName, timeElapsed, timeScale);
            System.out.printf(
                "\nWidth: %dpx | Height: %dpx | Samples per Pixel: %d\n",
                imageWidth, imageHeight, samplesPerPixel
            );

        } catch (IOException e) {
            System.out.printf("Error while writing to '%s'.\n", fileName);
        }

        bob.close();
    }

    // function to figure out what color the ray returns
    public static Color rayColor(Ray r, Color background, Hittable world, int depth) {
        HitRecord rec = new HitRecord();

        // recursion safeguard
        if (depth <= 0) {
            return new Color(0, 0, 0); // black
        }

        // Check if ray hits anything, if not return the background color
        if (!world.hit(r, 0.001, Utility.Infinity, rec))
            return background;

        Ray scattered = new Ray();
        Color attenuation = new Color();
        Color emitted = rec.getMaterial().emitted(rec.getU(), rec.getV(), rec.getP());

        // if ray doesn't get scattered return emitted color
        if (!rec.getMaterial().scatter(r, rec, attenuation, scattered))
            return emitted;

        // return emitted + attenuation * ray_color(scattered, background, world, depth-1);
        return Vec3.mul(
            Vec3.add(emitted, attenuation),
            rayColor(scattered, background, world, depth - 1)
        ).toColor();
    }

    // function to generate a color String, originally in color.py
    public static String writeColor(Color pixelColor, int samplesPerPixel) {
        double r = pixelColor.x();
        double g = pixelColor.y();
        double b = pixelColor.z();

        // divide the color by the number of samples and gamma correct for gamma = 2
        // -> raising color to the power 1/gamma -> 1/2 -> sqrt

        double scale = (double) 1 / samplesPerPixel;
        r = Math.sqrt(scale * r);
        g = Math.sqrt(scale * g);
        b = Math.sqrt(scale * b);

        // return the translated (0 to 256) value of each component
        return String.format("%d %d %d\n",
            (int) (256 * Utility.clamp(r, 0, 0.999)),
            (int) (256 * Utility.clamp(g, 0, 0.999)),
            (int) (256 * Utility.clamp(b, 0, 0.999))
        );
    }

    public static int writeColorBMP(Color pixelColor, int samplesPerPixel) {
        double r = pixelColor.x();
        double g = pixelColor.y();
        double b = pixelColor.z();

        // divide the color by the number of samples and gamma correct for gamma = 2
        // -> raising color to the power 1/gamma -> 1/2 -> sqrt

        double scale = (double) 1 / samplesPerPixel;
        r = Math.sqrt(scale * r);
        g = Math.sqrt(scale * g);
        b = Math.sqrt(scale * b);

        int rgb = (int) (256 * Utility.clamp(r, 0, 0.999));
        rgb = (rgb << 8) | (int) (256 * Utility.clamp(g, 0, 0.999));
        rgb = (rgb << 8) | (int) (256 * Utility.clamp(b, 0, 0.999));

        return rgb;
    }
}
