

// winrott.h

// husk at rette i winrott.c

extern int iGLOBAL_SCREENWIDTH;	 // bna
extern int iGLOBAL_SCREENHEIGHT; // bna
extern int iGLOBAL_SCREENBWIDE;
extern int iG_SCREENWIDTH; // default screen width in bytes

extern int iGLOBAL_HEALTH_X;
extern int iGLOBAL_HEALTH_Y;
extern int iGLOBAL_AMMO_X;
extern int iGLOBAL_AMMO_Y;

extern int iGLOBAL_FOCALWIDTH;
extern double dGLOBAL_FPFOCALWIDTH;

// Full-screen interfaces render at 320x200 and get stretched, the game screen
// renders at the real size. Both calls do nothing at 320x200.
void EnableScreenStretch(void);
void DisableScreenStretch(void);
