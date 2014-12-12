#include "video_gr.h"
#include "keyboard.h"

static char *video_mem;		/* Process address to which VRAM is mapped */
static long vram_size;		/* Size of VRAM */
static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u r;

	mmap_t map;

	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) //alloc memory needed
	{
		printf("Bad allocation of memory!\n");
		return 1;
	}
	r.u.w.ax = VBE_GET_MODE; /* VBE get mode info */
	r.u.w.es = PB2BASE(map.phys); /* set a segment base */
	r.u.w.di = PB2OFF(map.phys); /* set the offset accordingly */
	r.u.w.cx = mode; /* the mode required*/
	r.u.b.intno = INTERRUPT_VBE; /* interrupt VBE */
	if( sys_int86(&r) != OK ) { /* call BIOS */

		printf("Error. Video Mode Wrong!\n");
		lm_free(&map); //free memory
		return 1;
	}

	if (r.u.b.ah == 0x01 )
	{
		printf("Function call mapping failed!\n");
		lm_free(&map);
		return 1;
	}
	else if (r.u.b.ah == 0x02)
	{
		printf("Function is not supported in current HW configuration!\n");
		lm_free(&map);
		return 1;
	}
	else if (r.u.b.ah == 0x03)
	{
		printf("Function is invalid in current video mode!\n");
		lm_free(&map);
		return 1;
	}

	*vmi_p = *(vbe_mode_info_t*) map.virtual; //virtual memory
	lm_free(&map); //free memory
	return 0;
}

int get_vbe_info(vbe_info_block *vib_p){

	struct reg86u r;

	mmap_t map;

	if (lm_alloc(sizeof(vbe_info_block), &map) == NULL) //alloc memory needed
	{
		printf("Bad allocation of memory!\n");
		return 1;
	}

	r.u.w.ax = VBE_CONTROL_INFO; /* VBE control info */
	r.u.w.es = PB2BASE(map.phys); /* set a segment base */
	r.u.w.di = PB2OFF(map.phys); /* set the offset accordingly */
	r.u.b.intno = INTERRUPT_VBE; /* interrupt VBE */

	if( sys_int86(&r) != OK ) { /* call BIOS */

		printf("Error. Video Mode Wrong!\n");
		lm_free(&map); //free memory
		return 1;
	}

	*vib_p = *(vbe_info_block*) map.virtual; //virtual memory
	lm_free(&map); //free memory
	return 0;
}


int vbe_set_mode(unsigned short function, unsigned short mode) {


	struct reg86u r;

	r.u.w.ax = function; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = LINEAR_MODEL_BIT | mode; // set bit 14: linear framebuffer
	r.u.b.intno = INTERRUPT_VBE; // interrupt VBE (0x10)


	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}
	if (r.u.b.ah == 0x01 )
	{
		printf("Function call failed!\n");
		return 1;
	}
	else if (r.u.b.ah == 0x02)
	{
		printf("Function is not supported in current HW configuration!\n");
		return 1;
	}
	else if (r.u.b.ah == 0x03)
	{
		printf("Function is invalid in current video mode!\n");
		return 1;
	}

	return 0;
}

char *read_xpm(char *map[], int *wd, int *ht)
{
	int width, height, colors;
	char sym[256];
	int  col;
	int i, j;
	char *pix, *pixtmp, *tmp, *line;
	char symbol;

	/* read width, height, colors */
	if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}
#ifdef DEBUG
	printf("%d %d %d\n", width, height, colors);
#endif
	if (width > h_res || height > v_res || colors > 256) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}

	*wd = width;
	*ht = height;

	for (i=0; i<256; i++)
		sym[i] = 0;

	/* read symbols <-> colors */
	for (i=0; i<colors; i++) {
		if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
			printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
			return NULL;
		}
#ifdef DEBUG
		printf("%c %d\n", symbol, col);
#endif
		if (col > 256) {
			printf("read_xpm: incorrect color at line %d\n", i+1);
			return NULL;
		}
		sym[col] = symbol;
	}

	/* allocate pixmap memory */
	pix = pixtmp = malloc(width*height);

	/* parse each pixmap symbol line */
	for (i=0; i<height; i++) {
		line = map[colors+1+i];
#ifdef DEBUG
		printf("\nparsing %s\n", line);
#endif
		for (j=0; j<width; j++) {
			tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
			if (tmp == NULL) {
				printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
				return NULL;
			}
			*pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
			printf("%c:%d ", line[j], tmp-sym);
#endif
		}
	}

	return pix;
}

int xpm_cre(int *altura, int *largura, unsigned short x, unsigned short y, char* xpm[]) {

	int i = 0;
	int j = 0;

	char *video_copy = getVideoMem();
	unsigned h_res = getHRes();

	video_copy = video_copy + h_res * y + x;

	char* pixel;
	if (( pixel= read_xpm(xpm, largura, altura)) == NULL)
	{
		return 1;
	}

	else
	{
		for (i; i < *altura; i++)
		{
			for (j; j < *largura; j++)
			{
				*video_copy = *pixel;
				video_copy++;
				pixel++;
			}
			video_copy = video_copy + h_res - *largura;
			j = 0;
		}
	}
	return 0;
}

int xpm_del(int *altura, int *largura, unsigned short x, unsigned short y) {

	int i = 0;
	int j = 0;

	char *video_copy = getVideoMem();
	unsigned h_res = getHRes();

	video_copy = video_copy + h_res * y + x;

	for (i; i < *altura; i++)
	{
		for (j; j < *largura; j++)
		{
			*video_copy = 0;
			video_copy++;
		}
		video_copy = video_copy + h_res - *largura;
		j = 0;
	}
	return 0;
}


int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00;    /* Set Video Mode function */
	reg86.u.b.al = 0x03;    /* 80x25 text mode*/

	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vg_init(unsigned short mode) {


	vbe_mode_info_t config;

	if ( vbe_set_mode(VBE_MODE, mode) == 1) //set graphic mode
		return NULL;

	if (vbe_get_mode_info(mode, &config) != 0) //get vbe info
	{
		return NULL;
	}

	h_res = config.XResolution; //store X Resolution
	v_res = config.YResolution; //store Y Resolution
	bits_per_pixel = config.BitsPerPixel; //store Bits Per Pixel
	vram_size = (config.XResolution * config.YResolution * config.BitsPerPixel) / 8; //store the size of the vram


	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = (config.PhysBasePtr);
	mr.mr_limit = mr.mr_base + (config.XResolution * config.YResolution * config.BitsPerPixel) / 8;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, (config.XResolution * config.YResolution * config.BitsPerPixel) / 8);

	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");


	if (video_mem == NULL)
	{
		printf("vg_init: Error!\n");
		vg_exit();
		return NULL;
	}
	return 0;
}

char* getVideoMem(){

	return video_mem;
}

unsigned getHRes(){

	return h_res;
}

unsigned getVRes(){

	return v_res;
}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	int r;
	int ipc_status;
	message msg;
	int i = 0;
	int j = 0;
	int key = 0;
	char irq_set = BIT(macro_hook_id_keyboard);

	// inicializa em modo gráfico e retorna video_mem

	printf("irq_set: %x\n", irq_set);
	unsigned h_res = getHRes();
	unsigned v_res = getVRes();
	unsigned char *video_copy;
	video_copy = getVideoMem();

	if ((x >= h_res) || (x < 0) || (y >= v_res) || (y < 0)
			|| ((x + size) >= h_res) || ((y + size) >= v_res) || (size <= 0)) // o utilizador introduziu coordenadas erradas
	{
		return 1;
	}

	video_copy = video_copy + (h_res * y) + x; //coloca o endereço virtual na posição em que o utilizador pretende que comece

	for (i; i < size; i++) // pinta linhas verticais
	{
		for (j; j < size; j++) // pinta linhas horizontais
		{
			*video_copy = color;
			video_copy++;
		}
		j = 0;
		video_copy = video_copy + h_res - size; // linha seguinte
	}

	while (key != KBD_ESC_KEY) // condição de saída
	{
		printf("WHILE\n");
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		printf("DEPOIS\n");
		if (is_ipc_notify(ipc_status)) { /* received notification */
			printf("DEPOIS DEPOIS DEPOIS\n");
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					printf("ANTES DA KEY\n");/* subscribed interrupt */
					kbd_scan_c(&key);
					printf("key: %x\n", key);
				}
			}
		}
	}

	return 0;
}