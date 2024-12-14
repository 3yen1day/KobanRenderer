//�萔��`
#define WINDOW_WIDTH 960//�E�B���h�E��
#define WINDOW_HEIGHT 640 //�E�B���h�E����
#define APP_NAME L"KOBAN_RENDERER"
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}
#define MAX_Light 100

//GlobalAccess�n
#define GA Koban::GlobalAccess
#define RENDER GA::getRender()
#define DEVICE RENDER->getDevice()
#define DEVICE_CONTEXT RENDER->getDeviceContext()

#define SCENE GA::getScene()