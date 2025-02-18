

// winrott.h


//husk at rette i winrott.c

#define ORIGWIDTH 320
#define ORIGHEIGHT 200
#define SCREENWIDTH 640
#define SCREENHEIGHT 400

extern int iGLOBAL_SCREENWIDTH;//bna
extern int iGLOBAL_SCREENHEIGHT;//bna
extern int iGLOBAL_SCREENBWIDE ;
extern int iG_SCREENWIDTH;// default screen width in bytes

extern int iGLOBAL_HEALTH_X;
extern int iGLOBAL_HEALTH_Y;
extern int iGLOBAL_AMMO_X;
extern int iGLOBAL_AMMO_Y;

extern int iGLOBAL_FOCALWIDTH;
extern double dGLOBAL_FPFOCALWIDTH;

void EnableScreenStretch(void);
void DisableScreenStretch(void);

