//=============================================================================
// Copyright © 2017 FLIR Integrated Imaging Solutions, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of FLIR
// Integrated Imaging Solutions, Inc. ("Confidential Information"). You
// shall not disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with FLIR Integrated Imaging Solutions, Inc. (FLIR).
//
// FLIR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. FLIR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================

#ifndef __LADYBUGRENDERER_H__
#define __LADYBUGRENDERER_H__

/** 
 * @defgroup Ladybugrenderer_h ladybugrenderer.h
 *
 * ladybugrenderer.h
 *
 * This file defines a set of Ladybug functions that are intended for
 * rendering images directly to a on-screen frame buffer or off-screen
 * frame buffer.
 *
 * We welcome your bug reports, suggestions, and comments: 
 * www.ptgrey.com/support/contact
 */

/*@{*/

#ifdef __cplusplus
extern "C"
{
#endif

#include "ladybug.h"
#include "ladybuggeom.h"

/**  @defgroup Enumerations Enumerations */

/*@{*/ 

/** This enumeration describes Ladybug output image types. */
typedef enum
{
   /** Decompressed and color processed (unrectified) images */
   LADYBUG_RAW_CAM0          = ( 0x1 << 0 ),
   LADYBUG_RAW_CAM1          = ( 0x1 << 1 ),
   LADYBUG_RAW_CAM2          = ( 0x1 << 2 ),
   LADYBUG_RAW_CAM3          = ( 0x1 << 3 ),
   LADYBUG_RAW_CAM4          = ( 0x1 << 4 ),
   LADYBUG_RAW_CAM5          = ( 0x1 << 5 ),
   LADYBUG_ALL_RAW_IMAGES     =  0x0000003F,
   
   /** Rectified images */
   LADYBUG_RECTIFIED_CAM0     = ( 0x1 << 6 ),
   LADYBUG_RECTIFIED_CAM1     = ( 0x1 << 7 ),
   LADYBUG_RECTIFIED_CAM2     = ( 0x1 << 8 ),
   LADYBUG_RECTIFIED_CAM3     = ( 0x1 << 9 ),
   LADYBUG_RECTIFIED_CAM4     = ( 0x1 << 10 ),
   LADYBUG_RECTIFIED_CAM5     = ( 0x1 << 11 ),
   LADYBUG_ALL_RECTIFIED_IMAGES     =  0x00000FC0,
   
   /** Panoramic image */
   LADYBUG_PANORAMIC          = ( 0x1 << 12 ),         
   
   /** Dome projection image */
   LADYBUG_DOME              = ( 0x1 << 13 ),
   
   /** Spherical image */
   LADYBUG_SPHERICAL          = ( 0x1 << 14 ),
   
   /** All decompressed and color processed images in one view */
   LADYBUG_ALL_CAMERAS_VIEW   = ( 0x1 << 15 ),
   
   /** All output images */
   LADYBUG_ALL_OUTPUT_IMAGE   = 0x7FFFFFFF,
   
} LadybugOutputImage;

/** A record used in querying Ladybug image rendering information. */
typedef struct LadybugImageRenderingInfo
{
   /** Video card device description */
   char pszDeviceDescription[ 128 ];

   /** Video card adapter string */
   char pszAdapterString[ 128 ];

   /** BIOS version string */
   char pszBiosString[ 128 ];

   /** Video card chip type */
   char pszChipType[ 128 ];

   /** Video card digital-to-analog converter type */
   char pszDacType[ 128 ];

   /** Video card installed display driver */
   char pszInstalledDisplayDriver[ 128 ];

   /** Video card driver version string */
   char pszDriverVersion[ 64 ];

   /** Video card memory size */
   unsigned int uiMemorySize;

   /** OpenGL version string */
   char pszOpenGLVersion[ 64 ];

   /** Indicates if Pixel Buffer Object is supported  */
   bool bPBO;

   /** Indicates if Frame Buffer Object is supported */
   bool bFBO;

   /** OpenGL max texture width or height */
   unsigned int uiMaxTextureSize;

   /** OpenGL max view port width */
   unsigned int uiMaxViewPortWidth;

   /** OpenGL max view port height */
   unsigned int uiMaxViewPortHeight;

   /** OpenGL max render buffer size */
   unsigned int uiMaxRenderbufferSize;

   /** The company responsible for this OpenGL implementation. */
   char pszOpenGLVendor[ 64 ];

   /** The name of the OpenGL renderer of the hardware platform */
   char pszOpenGLRenderer[ 64 ];

   /** Indicates if pixel buffer is supported  */
   bool bPBuffer;

   unsigned int reserved[ 979 ];

} LadybugImageRenderingInfo;

/*@}*/ 

/** 
 * @defgroup RendererGeneralFunctions Renderer General Functions
 *
 * These functions are used for on-screen display 
 * and off-screen rendering.
 */

/*@{*/ 

/**
 * Configures the Ladybug library for generating Ladybug output images for
 * on-screen and off-screen rendering.
 *
 * This function must be called after loading the configuration file by 
 * calling ladybugLoadConfig().  
 *
 * This function must be called prior to calling any of the following API
 * functions: ladybugSetDisplayWindow(), ladybugDisplayImage(), 
 * ladybugRenderOffScreenImage(), ladybugGetOpenGLTextureID().
 *
 * Image types LADYBUG_RAW_CAM0, LADYBUG_RAW_CAM1, ..., LADYBUG_RAW_CAM5 are 
 * used to identify the processed camera images that are updated by calling
 * ladybugUpdateTextures(). They are not valid parameters for this function.
 *
 * LADYBUG_ALL_CAMERAS_VIEW is supported only for on-screen rendering. It is
 * not supported for off-screen rendering.
 *
 * This function configures the Ladybug library for software off-screen
 * rendering if ladybugEnableSoftwareRendering() is called prior 
 * to this function.
 *
 * For example, if the application needs to display panoramic images on the
 * screen and generate off-screen dome projection images, call 
 * ladybugConfigureOutputImages(context, LADYBUG_PANORAMIC | LADYBUG_DOME).
 *
 * @param context      - The LadybugContext to access.
 * @param uiImageTypes - The combination of Ladybug output image types defined in 
 *                       LadybugOutputImage. The valid output image types are:
 *                       LADYBUG_PANORAMIC,
 *                       LADYBUG_DOME,
 *                       LADYBUG_SPHERICAL,
 *                       LADYBUG_RECTIFIED_CAM0,
 *                       LADYBUG_RECTIFIED_CAM1,
 *                       LADYBUG_RECTIFIED_CAM2,
 *                       LADYBUG_RECTIFIED_CAM3,
 *                       LADYBUG_RECTIFIED_CAM4,
 *                       LADYBUG_RECTIFIED_CAM5,
 *                       LADYBUG_ALL_RECTIFIED_IMAGE,
 *                       LADYBUG_ALL_CAMERAS_VIEW
 *
 * @return LADYBUG_OK is returned if all the specified image types are successfully
 *   configured. If any unsupported image types are specified by uiImageTypes,
 *   it returns LADYBUG_INVALID_ARGUMENT.
 *
 * @see   ladybugLoadConfig(),
 *   ladybugSetDisplayWindow(),
 *   ladybugDisplayImage(),
 *   ladybugRenderOffScreenImage(),
 *   ladybugGetOpenGLTextureID(),
 *   ladybugEnableSoftwareOffScreenRendering()
 */
LADYBUGDLL_API LadybugError 
ladybugConfigureOutputImages(
            LadybugContext  context,
            unsigned int    uiImageTypes );

/**
 * Returns information about the graphics card and OpenGL implementation.
 *
 * @param context        - The LadybugContext to access.
 * @param pRenderingInfo - Location to return the requested information. 
 *                         It is a pointer to LadybugImageRenderingInfo structure.
 *
 * @return   A LadybugError indicating the success of the function.
 */
LADYBUGDLL_API LadybugError 
ladybugGetImageRenderingInfo(
        LadybugContext context,
        LadybugImageRenderingInfo* pRenderingInfo );

/**
 * Updates the Ladybug image texture buffers on the GPU with the images 
 * specified by arpBGRABuffers.
 *
 * If ladybugConvertImage() is used to convert images to 
 * internal image buffers, arpBGRABuffers must be specifed as NULL to update 
 * these internal images to the GPU buffers.
 *
 * If arpBGRABuffers is specified as NULL and there are no internal image 
 * buffers, this function returns LADYBUG_INVALID_ARGUMENT.
 *
 * This function only needs to be called once per re-draw, even if there are 
 * multiple OpenGL instances in different windows.
 *
 * @param context        - The LadybugContext to access.
 * @param uiCameras      - The number of buffers in the array of the following 
 *                         argument. Should be LADYBUG_NUM_CAMERAS.
 * @param arpBGRABuffers - An array of pointers to the BGRA image buffers to be 
 *                         loaded onto the GPU texture buffers. If NULL is specified, 
 *                         this function loads the images that are in the internal 
 *                         buffers onto the GPU texture buffers. 
 * @param pixelFormat    - The pixel format of BGRA image buffers.
 *
 * @return A LadybugError indicating the success of the function. 
 *
 * @see ladybugConvertImage(), ladybugSetDynamicStitching()
 */
LADYBUGDLL_API LadybugError 
ladybugUpdateTextures( 
                        LadybugContext          context,
                        unsigned int            uiCameras,
                        const unsigned char**   arpBGRABuffers,
                        LadybugPixelFormat      pixelFormat = LADYBUG_BGRU);

/**
* Updates the Ladybug image texture buffers on the GPU with the images
* specified by arpRGBABuffers.
*
* Same as ladybugUpdateTextures() but with GPU pointers pointing to the 
* location of the images on an available GPU. Uses RGBA buffers instead of the BGRA buffers
* in ladybugUpdateTextures(). 
*
* @param context        - The LadybugContext to access.
* @param uiCameras      - The number of buffers in the array of the following
*                         argument. Should be LADYBUG_NUM_CAMERAS.
* @param arpRGBABuffers - An array of GPU pointers to the RGBA image buffers to be
*                         loaded onto the GPU texture buffers. If NULL is specified,
*                         this function loads the images that are in the internal
*                         buffers onto the GPU texture buffers.
*
* @return A LadybugError indicating the success of the function.
*
* @see ladybugConvertImage(), ladybugSetDynamicStitching()
*/
LADYBUGDLL_API LadybugError
ladybugUpdateTexturesGPU(
                        LadybugContext          context,
                        unsigned int            uiCameras,
                        const unsigned char**   arpRGBABuffers,
                        LadybugPixelFormat      pixelFormat = LADYBUG_RGBU);

/**
 * Specify the row and column values for Ladybug image 3D mapping. These
 * values are the resolution of the 3D mapping coordinates to the sphere used 
 * in Ladybug library for spherical view, panoramic view, and dome view image
 * stitching. 
 *
 * For panoramic, sphere and dome views, the Ladybug library uses 3D grids to
 * map Ladybug images to a 3D sphere. This function sets how many rows and
 * columns are in these 3D grids. By default, the Ladybug library uses a 
 * 128x128 grid size for all mappings. This function is called only if 
 * users want to use other grid sizes than the default size.
 *
 * This function has to be called prior to calling 
 * ladybugConfigureOutputImages().
 *
 * @param context    - The LadybugContext to access.
 * @param uiGridCols - Columns in the 3D grids.
 * @param uiGridRows - Rows in the 3D grids.
 *
 * @return A LadybugError indicating the success of the function.
 */
LADYBUGDLL_API LadybugError
ladybugSet3dMapSize( 
        LadybugContext  context,
        unsigned int    uiGridCols,
        unsigned int    uiGridRows );

/**
 * Gets the row and column values of the 3D mapping grids.
 *
 * @param context     - The LadybugContext to access.
 * @param puiGridCols - Returned columns in the 3D grids.
 * @param puiGridRows - Returned rows in the 3D grids.
 *
 * @return A LadybugError indicating the success of the function.
 */
LADYBUGDLL_API LadybugError
ladybugGet3dMapSize( 
        LadybugContext  context,
        unsigned int*   puiGridCols,
        unsigned int*   puiGridRows );

/**
 * The Ladybug library uses 2D mesh grids to map Ladybug images to rectified
 * images. This function sets the number of rows and columns in the mapping
 * mesh.
 *
 * By default, the Ladybug library uses a 256x192 mapping mesh to render the
 * rectified images. This function is called only if the user wants to use
 * mesh sizes other than the default size. This function has to be 
 * called prior to calling ladybugConfigureOutputImages().
 *
 * This function has to be called after loading the configuration file 
 * by calling ladybugLoadConfig().  
 *
 * The minimum value for either uiMeshCols or uiMeshRows is 4. The maximum 
 * value of uiMeshCols is the columns in the raw (colour) source image. The 
 * maximum value of uiMeshRows is the rows in the raw (colour) source image.
 *
 * For example, if the raw image size is 1024x768 and the application needs
 * to set the rectified mapping mesh size to 512x384, call 
 * ladybugSetRectifyMeshResolution(context, 512, 384).
 *
 * @param   context    - The LadybugContext to access.
 * @param  uiMeshRows - Rows of the mapping mesh.
 * @param  uiMeshCols - Columns of the mapping mesh.
 *
 * @return   A LadybugError indicating the success of the function.
 *
 * @see   ladybugConfigureOutputImages(), ladybugGetRectifyMeshResolution()
 */
LADYBUGDLL_API LadybugError
ladybugSetRectifyMeshResolution( 
        LadybugContext  context,
        unsigned int    uiMeshRows,
        unsigned int    uiMeshCols );

/**
 * Gets the number of rows and columns of the rectified image
 * mapping mesh.
 *
 * @param context     - The LadybugContext to access.
 * @param puiMeshRows - The returned rows of the mapping mesh.
 * @param puiMeshCols - The returned columns of the mapping mesh.
 *
 * @return A LadybugError indicating the success of the function.
 *
 * @see ladybugSetRectifyMeshResolution()
 */
LADYBUGDLL_API LadybugError
ladybugGetRectifyMeshResolution(
        LadybugContext  context,
        unsigned int*   puiMeshRows,
        unsigned int*   puiMeshCols);

/**
 * Changes the viewing angle of the dome view.
 *
 * The viewing angle is defined by the radial coordinate Phi. It ranges from
 * zero (up) to 180 (down). The default viewing angle is 180.
 *
 * If uiAngle is 90, then the projection generates hemispherical images.
 * If uiAngle is 180, then the projection generates a full dome image.
 *
 * @param context - The LadybugContext to access.
 * @param uiAngle - The viewing angle to set.
 *
 * @return A LadybugError indicating the success of the function.
 */
LADYBUGDLL_API LadybugError 
ladybugChangeDomeViewAngle( 
                           LadybugContext context,
                           unsigned int      uiAngle );

/**
 * Retrieves the dome viewing angle as set by ladybugChangeDomeViewAngle().
 *
 * @return A LadybugError indicating the success of the function.
 */
LADYBUGDLL_API LadybugError 
ladybugGetDomeViewAngle( 
                           LadybugContext context,
                           unsigned int* puiAngle );

/**
 * Sets the spherical view transformation parameters for subsequent 
 * rendering function calls.
 *
 * If this function is not called, the rendering functions 
 * ladybugDisplayImage() and ladybugRenderOffScreenImage() will render 
 * the spherical image with camera0 in front and the viewing point 
 * located at the center of the 6 cameras by default. It is important to note that
 * the transformation set by this function is applied upon these
 * default transformation settings.
 *
 * The rotation follows the right-hand rule, so if the rotation vector  
 * points toward the user, the rotation will be counter clockwise.
 * The rotations are applied in the order of fRotZ, fRotY, fRotX.  
 *
 * fTransX, fTransY, fTransZ are used to move the rendered image.
 * Note that if the specified value is bigger than the sphere size-- 
 * 20 for a camera with 20-meter calibration, for example, the image 
 * may be out of view.
 * 
 * Rotation transformations are applied first, followed by 
 * translations.
 *
 * The rotation and translation set by this function are 
 * always applied as model transformations on the rendered image. 
 * It is recommended to use either this function or OpenGL model 
 * transformation in the application; do not use both. 
 *
 * Each time this function is called, it overwrites the previous 
 * transformation set by this function.
 *
 * @param context - The LadybugContext to access.
 * @param fFOV    - The field-of-view angle, in degrees. The valid value is
 *                  between 0 and 180.
 * @param fRotX   - The angle of rotation about the X-axis, in radians.
 * @param fRotY   - The angle of rotation about the Y-axis, in radians.
 * @param fRotZ   - The angle of rotation about the Z-axis, in radians.
 * @param fTransX - The translation along the X-axis.   
 * @param fTransY - The translation along the Y-axis.   
 * @param fTransZ - The translation along the Z-axis.   
 *
 * @return   A LadybugError indicating the success of the function.
 *
 * @see   ladybugGetSphericalViewParams,
 *   ladybugDisplayImage(),
 *   ladybugRenderOffScreenImage()
 */
LADYBUGDLL_API LadybugError
ladybugSetSphericalViewParams(
                        LadybugContext context,  
                        float fFOV,
                        float fRotX, 
                        float fRotY, 
                        float fRotZ, 
                        float fTransX, 
                        float fTransY, 
                        float fTransZ );

/**
 * Gets the current spherical view transformation parameters.
 *
 * @param context  - The LadybugContext to access.
 * @param pfFOV    - The field-of-view angle, in degrees.
 * @param pfRotX   - The angle of rotation about the X-axis, in radians.
 * @param pfRotY   - The angle of rotation about the Y-axis, in radians.
 * @param pfRotZ   - The angle of rotation about the Z-axis, in radians.
 * @param pfTransX - The translation along the X-axis.   
 * @param pfTransY - The translation along the Y-axis.   
 * @param pfTransZ - The translation along the Z-axis.   
 *
 * @return A LadybugError indicating the success of the function.
 *
 * @see ladybugSetSphericalViewParams()
 */
LADYBUGDLL_API LadybugError
ladybugGetSphericalViewParams(
                        LadybugContext context,  
                        float *pfFOV,
                        float *pfRotX, 
                        float *pfRotY, 
                        float *pfRotZ, 
                        float *pfTransX, 
                        float *pfTransY, 
                        float *pfTransZ );

/**
 * Enables the anti-aliasing feature of the Ladybug library. If enabled,
 * the six texture images that are updated by calling 
 * ladybugUpdateTextures() are processed to minimize sampling artifacts
 * that may appear on the rendered images.
 * 
 * This feature is recommended when rendering small size output images.
 *
 * Enabling or disabling anti-aliasing takes effect on the next call of 
 * ladybugUpdateTextures(). This feature is disabled by default
 * in the Ladybug library.
 *
 * If anti-aliasing is enabled, the rendered images may appear
 * blurry.
 * 
 * An on-screen image is rendered by ladybugDisplayImage() and an off-screen
 * image is rendered and returned by ladybugRenderOffScreenImage().
 *
 * Testing indicates that anti-aliasing may produce unexpected results 
 * on ATI graphics cards that support OpenGL version 3.0 or earlier. 
 *
 * For example to enable/disable anti-aliasing, call these Ladybug API 
 * functions in the following order:
 * ladybugSetAntiAliasing(true/false);
 * ladybugUpdateTextures();
 * ladybugDisplayImage() or ladybugRenderOffScreenImage();
 *
 * @param context - The LadybugContext to access.
 * @param bEnable - Specifies whether to enable anti-aliasing. The default value 
 *                  is off.
 *
 * @return A LadybugError indicating the success of the function.
 *
 * @see ladybugUpdateTextures(), ladybugDisplayImage(), ladybugRenderOffScreenImage()
 */
LADYBUGDLL_API LadybugError 
ladybugSetAntiAliasing( 
    LadybugContext context,
    bool bEnable );

/*@}*/ 

/** 
 * @defgroup RendererOnscreenRenderingFunctions On-screen Rendering Functions
 *
 * These functions are used for configuring on-screen display of Ladybug images. 
 */

/*@{*/ 

/**
 * Initializes an on-screen window for displaying Ladybug images. This
 * function must be called prior to calling ladybugDisplayImage().
 *
 * This function is used to initialize the Ladybug library and the 
 * display window. Prior to calling this function, the display window must
 * have a valid current OpenGL rendering context. The properties of the pixel
 * buffer for the rendering context must be specified as PFD_SUPPORT_OPENGL,
 * PFD_DRAW_TO_WINDOW and PFD_DOUBLEBUFFER. The pixel encoding format must be
 * specified as PFD_TYPE_RGBA. The number of color bit planes must be 24. For
 * more information about how to create a rendering context, refer to the 
 * ChoosePixelFormat(), SetPixelFormat(), wglCreateContext(), wglMakeCurrent()
 * and PIXELFORMATDESCRIPTOR topics in the Microsoft Win32 OpenGL library 
 * documentation. 
 * (http://msdn.microsoft.com/en-us/library/ms673957(VS.85).aspx)
 *
 * When this function is called, the current OpenGL rendering context 
 * must not contain any existing display lists, textures, Pixel Buffer
 * Objects(PBO), Vertex Buffer Objects(VBO) or Frame Buffer Objects(FBO).
 *
 * This function may only be called once per OpenGL rendering context
 * unless directly preceeded by a call to ladybugConfigureOutputImages()
 * which resets internal OpenGL state.
 *
 * @param context - The LadybugContext to access.
 *
 * @return A LadybugError indicating the success of the function.
 *
 */
LADYBUGDLL_API LadybugError 
ladybugSetDisplayWindow( LadybugContext context );

/**
 * Displays a Ladybug image in a screen window. 
 *
 * Prior to calling this function, the display window must have a valid 
 * current OpenGL rendering context. If the output image type is specified as 
 * LADYBUG_SPHERICAL, in order to display the spherical image in the window, 
 * the application has to set OpenGL viewing transformation properly.
 *
 * @param context   - The LadybugContext to access.
 * @param imageType - Output image type to be displayed on the screen window.
 *                    Valid image types are defined in LadybugOutputImage.
 *
 * @return A LadybugError indicating the success of the function.
 *
 * @see ladybugConfigureOutputImages(), ladybugSetDisplayWindow()
 */
LADYBUGDLL_API LadybugError 
ladybugDisplayImage(
            LadybugContext      context,
            LadybugOutputImage  imageType );

/*@}*/ 

/** 
 * @defgroup RendererOffscreenRenderingFunctions Off-screen Rendering Functions
 *
 * These functions are used for off-screen image rendering. 
 */

/*@{*/ 

/**
 * Sets the off-screen image size. An off-screen image is generated using 
 * the graphics card. This function is used to set the size of the rendering
 * buffer.
 *
 * This function is optional. If it is not called, the library will use
 * default values to generate the off-screen image. For panoramic
 * images, the default size is 2048x1024. For dome view images, the default
 * size is 1024x1024. For rectified images, the default size is the size
 * of the texture being transferred.
 *
 * If this function is used to change the size of an off-screen image that
 * already has been initialized, the renderer will be reinitialized. As a
 * result, the texture ID of the next rendered image may not be the same.
 * Call ladybugGetOpenGLTextureID() again to ensure that the proper
 * texture ID is obtained.
 *
 * The maximum allowed width and height of the off-screen image depends 
 * on the OpenGL implementation of the graphics card. The width of 
 * the off-screen image can be set as much as twice the maximum 
 * OpenGL view port width. The height of the off-screen image can 
 * be set as much as the maximum OpenGL view port height. For example,
 * if the maximum viewport size is 4096 x 4096, the maximum allowed
 * off-screen image size is 8192 x 4096. If the width of the off-screen
 * image is bigger than the maximum OpenGL view port width, the value of 
 * uiCols must be a multiple of 4. 
 * 
 * The graphics card rendering information, including OpenGL view port size,
 * can be retrieved by calling ladybugGetImageRenderingInfo().
 *
 * This function also sets the resolution of rectified images if the 
 * imageType argument is specified as any one of the rectified images. 
 * For this purpose, this function must be called prior to calls to 
 * ladybugUnrectifyPixel(), ladybugRectifyPixel(), 
 * ladybugGetCameraUnitFocalLength() and ladybugGetCameraUnitImageCenter() 
 * as an initialization step.
 *
 * Image types LADYBUG_RAW_CAM0, LADYBUG_RAW_CAM1, ..., LADYBUG_RAW_CAM5 are 
 * used to identify the processed camera images that are updated by calling
 * ladybugUpdateTextures(). They are not valid parameters for this function.
 *
 * LADYBUG_ALL_CAMERAS_VIEW is supported only for on-screen rendering. It is
 * not supported for off-screen rendering.
 *
 * @param context   - The LadybugContext to access.
 * @param imageType - The type of output image to be set.
 * @param uiWidth   - The width of the off-screen image (in pixels). 
 * @param uiHeight  - The height of the off-screen image (in pixels).
 *
 * @return A LadybugError indicating the success of the function.
 *
 * @see ladybugGetOffScreenImageSize(),
 *   ladybugRenderOffScreenImage(),
 *   ladybugReleaseOffScreenImage(),
 *   ladybugGetOpenGLTextureID(),
 *   ladybugGetImageRenderingInfo(),
 *   ladybugUnrectifyPixel(),
 *   ladybugRectifyPixel(), 
 *   ladybugGetCameraUnitFocalLength(),
 *   ladybugGetCameraUnitImageCenter()
 */
LADYBUGDLL_API LadybugError
ladybugSetOffScreenImageSize(
            LadybugContext      context,
            LadybugOutputImage  imageType,
            unsigned int        uiWidth,
            unsigned int        uiHeight );

/** 
 * Gets the size of the off-screen image.
 *
 * @param context   - The LadybugContext to access.
 * @param imageType - The type of the output image to access.
 * @param puiWidth  - Location to return the width of the image (in pixels).
 * @param puiHeight - Location to return the height of the image (in pixels).
 *
 * @return A LadybugError indicating the success of the function.
 *
 * @see ladybugSetOffScreenImageSize()
 */
LADYBUGDLL_API LadybugError
ladybugGetOffScreenImageSize(
            LadybugContext      context,
            LadybugOutputImage  imageType, 
            unsigned int*       puiWidth,
            unsigned int*       puiHeight );

/**
 * Renders an off-screen image and gets the image from the off-screen buffer.
 * The size of the image will be defined by the default value or can be set
 * by ladybugSetOffScreenImageSize() beforehand.
 *
 * This function renders the specified image to the off-screen frame buffer.
 * If pImage is NULL, this function will only render the image in the 
 * buffer and not return the rendered image. 
 *
 * The rendered off-screen image can also be accessed by using OpenGL 
 * texture ID. This can be done by calling ladybugGetOpenGLTextureID().
 *
 * This function does not return metadata for the rendered image. 
 * The metadata in LadybugProcessedImage struct to which pImage points remains
 * unchanged. If pImage is saved to disk in LADYBUG_FILEFORMAT_EXIF format, 
 * the metadata in LadybugProcessedImage must be filled in properly.
 *
 * @param context     - The LadybugContext to access.
 * @param imageType   - Type of image to be rendered, as defined in LadybugOutputImage.
 *                      The supported image types are:
 *                      LADYBUG_PANORAMIC,
 *                      LADYBUG_DOME,
 *                      LADYBUG_SPHERICAL,
 *                      LADYBUG_RECTIFIED_CAM0,
 *                      LADYBUG_RECTIFIED_CAM1,
 *                      LADYBUG_RECTIFIED_CAM2,
 *                      LADYBUG_RECTIFIED_CAM3,
 *                      LADYBUG_RECTIFIED_CAM4,
 *                      LADYBUG_RECTIFIED_CAM5
 * @param pixelFormat - Type of the pixel format for the offscreen image. RGB formats are not currently
 *                      supported.
 *                      For low dynamic range rendering, LADYBUG_BGR is recommended.
 *                      For high dynamic range rendering, LADYBUG_BGR16 or LADYBUG_BGR32F is recommended.
 * @param pImage      - A pointer to the output LadybugProcessedImage struct.
 *
 * @see ladybugGetOffScreenImageSize(),
 *   ladybugSetOffScreenImageSize(),
 *   ladybugGetOpenGLTextureID(),
 *   ladybugSaveImage().
 */
LADYBUGDLL_API LadybugError
ladybugRenderOffScreenImage(
             LadybugContext context,
             LadybugOutputImage imageType,
             LadybugPixelFormat pixelFormat,
             LadybugProcessedImage* pImage );

/**
 * Gets the OpenGL texture ID on the graphics card for the specified 
 * LadybugOutputImage type.
 *
 * If ppuiID is NULL, this function returns LADYBUG_INVALID_ARGUMENT.
 * 
 * If imageType argument is specified as one of the raw camera image types,
 * i.e., the enums from LADYBUG_RAW_CAM0 to LADYBUG_RAW_CAM5, 
 * this function can be called immediately after calling 
 * ladybugConfigureOutputImages(). If imageType argument is specified as
 * another image type, ladybugRenderOffScreenImage() must be called to
 * render the off screen image prior to calling this function. Otherwise, 
 * this function will return LADYBUG_INVALID_OPENGL_TEXTURE, because the
 * off-screen rendering resources have not been initialized yet.
 *
 * When this function is called, there must be a valid current OpenGL 
 * rendering context initialized by ladybugSetDisplayWindow(). We recommend
 * validating the returned texture pointed to by ppuiID by using
 * glIsTexture().
 *
 * The returned values pointed by pfROIWidth and pfROIHeight are used to 
 * specify the texture coordinates in the OpenGL glTexCoord*() functions.
 * These values specify the actual size of the texture image in the
 * texture buffer on the graphics card.
 *
 * This function will return LADYBUG_INVALID_OPENGL_TEXTURE if software 
 * rendering is enabled by ladybugEnableSoftwareRendering().
 * 
 * For example, if the width of the texture buffer is 1024, and the width
 * of the texture image is 512, then the fROIWidth value returned is
 * (512-1)/(1024-1)=0.499511. Correspondingly, if the height of the texture
 * buffer is 768, and the height of the texture image is 384, then the value
 * of fROIHeight is (384-1)/(768-1)=0.499348.
 *
 * @param context     - The LadybugContext to access.
 * @param imageType   - The type of image defined in LadybugOutputImage.
 *                      The following image types are not valid for this function
 *                      LADYBUG_ALL_RAW_IMAGES,
 *                      LADYBUG_ALL_RECTIFIED_IMAGES,
 *                      LADYBUG_ALL_OUTPUT_IMAGE
 * @param puiID       - Location to return the OpenGL texture ID of the image.
 * @param pfROIWidth  - Location to return the ratio of the width of the actual
 *                      texture image and the width of the allocated texture 
 *                      buffer. The returned value is always 1 if imageType 
 *                      argument is not specified as one of the enums from
 *                      LADYBUG_RAW_CAM0 to LADYBUG_RAW_CAM5.
 * @param pfROIHeight - Location to return the ratio of the height of the actual
 *                      texture image and the height of the allocated texture
 *                      buffer. The returned value is always 1 if imageType
 *                      argument is not specified as one of the enums from
 *                      LADYBUG_RAW_CAM0 to LADYBUG_RAW_CAM5.
 *
 * @see ladybugRenderOffScreenImage(),
 *   ladybugEnableSoftwareRendering(),
 *   ladybugSetDisplayWindow()
 */
LADYBUGDLL_API LadybugError
ladybugGetOpenGLTextureID(
    LadybugContext  context,
    LadybugOutputImage  imageType,
    unsigned int*  puiID,
    float*  pfROIWidth,
    float*  pfROIHeight );

/**
 * Releases the off-screen image rendering resources on the graphics card.
 *
 * Call this function to release the image rendering resources on the graphics
 * card. If it is not called, the Ladybug library will automatically release 
 * these resources when the Ladybug context is destroyed.
 *
 * @param context    - The LadybugContext to access.
 * @param imageTypes - The type of image to release.
 *
 * @return A LadybugError indicating the success of the function.
 */
LADYBUGDLL_API LadybugError
ladybugReleaseOffScreenImage(  
    LadybugContext  context,
    LadybugOutputImage  imageTypes );

/**
 * Enables the Ladybug library to render off-screen images without the use of 
 * hardware acceleration.
 *
 * This function is used to enable the Ladybug library to render off-screen 
 * Ladybug images by using a buffer in system memory. This means that
 * the image rendering process will not be hardware accelerated, although 
 * a graphics card might be currently installed.
 *
 * This function must be called prior to calling 
 * ladybugConfigureOutputImages().
 *
 * To enable software off-screen rendering, this function must be called, 
 * even if there is no OpenGL graphics card installed. 
 *
 * Once software off-screen rendering is enabled, the current Ladybug 
 * context cannot be used for displaying images in a window. Users 
 * cannot disable software off-screen rendering and reconfigure the
 * the current context for displaying images.
 *
 * Function ladybugGetOpenGLTextureID() will return 
 * LADYBUG_INVALID_OPENGL_TEXTURE when software rendering is enabled.
 *
 * This function cannot be used with Ladybug3 or newer cameras.
 *
 * @param context - The LadybugContext to access.
 * @param bEnable - Specifies whether to enable software rendering.
 *
 * @return A LadybugError indicating the success of the function.
 *
 * @see ladybugConfigureOutputImages(),
 *   ladybugRenderOffScreenImage(),
 *   ladybugSetOffScreenImageSize(),
 *   ladybugSetDisplayWindow(),
 *   ladybugGetOpenGLTextureID()
 */
LADYBUGDLL_API LadybugError 
ladybugEnableSoftwareRendering( 
                  LadybugContext context,
                  bool           bEnable );

/**
 * Enables adjustment of image intensity to compensate for exposure
 * differences between each sensor on the camera system.
 *
 * You need to call ladybugConfigureOutputImages() before this is called.
 *
 * This feature is useful when the camera is set to independent exposure mode and 
 * high dynamic range (HDR) rendering is used. 
 *
 * When the camera is in independent exposure mode, images from each camera 
 * are taken with different exposure settings, so the level of image intensity
 * may appear different across the borders of the stitched image. By enabling intensity adjustment,
 * image intensity from all cameras are normalized to the same scaling, and the 
 * stitched image looks more natural.
 *
 * This is also useful in conjunction with HDR
 * rendering (using floating point pixel format for ladybugRenderOffscreenImage).
 *
 * This functionality requires OpenGL version 2.0 or later which is not
 * provided by the software renderer (see ladybugEnableSoftwareRendering()).
 *
 * When enabled, exposure is calculated in ladybugConvertImage()
 * and the actual scaling is applied in the GPU.
 *
 * This functionality is only supported in Ladybug3 with firmware that 
 * supports independent exposure mode.
 *
 * @param context - The LadybugContext to access.
 * @param bEnable - Specifies whether to enable this functionality. The default 
 *                  value is off.
 *
 * @return A LadybugError indicating the success of the function.
 *
 * @see ladybugConvertImage(), ladybugRenderOffscreenImage()
 */
LADYBUGDLL_API LadybugError
ladybugSetTextureIntensityAdjustment(  
    LadybugContext context,
    bool bEnable);

/*@}*/

/*@}*/

#ifdef __cplusplus
};
#endif

#endif // #ifndef __LADYBUGRENDERER_H__
