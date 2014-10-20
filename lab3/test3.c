#include"test3.h"
#include"keyboard.h"



int kbd_test_scan(unsigned short ass) {
	int r;
	int key = 0;
	int return_byte;
	int ipc_status;
	int msb;
	msb = 0;
	message msg;
	char irq_set = BIT(khook_id);

	if (-1 == kbd_subscribe_int())
	{
		printf("Fail to subscribe Keyboard!\n\n");
		return 1;
	}

	while(0x81 != key) { /* When ESC key is pressed */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					if (return_byte != 1) //if return_byte == 1 then the key has 2 bytes
					{
						if (ass == 0)
							return_byte = kbd_scan_c(&key);
						else key = kbd_scan_ass();
						if (return_byte == 0)
						{
							if (0x0080 == (0x0080 & key))
							{
								printf("Breakcode: 0x%x\n", key);
							}
							else
							{
								//if (key == 0)
								{
									printf("Makecode: 0x%x\n", key);
								}
							}
						}
					}
					else
					{
						msb = key;
						msb = msb << 8;
						return_byte = kbd_scan_c(&key);
						key = key & 0x00FF;
						key = key + msb;
						if (0x0080 == (0x0080 & key))
						{
							printf("Breakcode: 0x%x\n", key);
						}
						else
						{
							//if (key == 0)
							{
								printf("Makecode: 0x%x\n", key);
							}
						}
						//return_byte = 0;
						//kbd_scan_c(&key);
					}
					/* process it */
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	kbd_unsubscribe_int();
	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *leds) {
	unsigned short i = 0;
	int l0, l1, l2;
	l0 = 0;
	l1 = 0;
	l2 = 0;

	if (-1 == kbd_subscribe_int())
	{
		printf("Fail to subscribe Keyboard!\n\n");
		return 1;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////
	int r; //vai ter o codigo do erro driver_receive(ANY, &msg, &ipc_status)
	int ipc_status;
	int temp_counter = 0; //contador de tiques
	unsigned long freq = 60; //frequencia normal
	message msg;

	timer_test_square(freq); //para funcionar a frequencia normal
	char irq_set = BIT(hook_id);

	if (-1 == timer_subscribe_int())
	{
		printf("Fail to subscribe Timer 0!\n\n");
		return 1;
	}
	for (i; i < n; i++)
	{
		global_counter = 0;
		while( global_counter < 1 ) { /* You may want to use a different condition */
			/* Get a request message. */
			if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
						temp_counter++;
						if ((temp_counter/freq) == 1) //se for true quer dizer que passou um segundo
						{
							//elem = leds[i];
							if (leds[i] == 0x0)
							{
								if (l0 == 0)
									l0 = 0x1;
								else l0 = 0;
							}
							else if(leds[i] == 0x1)
							{
								if (l1 == 0)
									l1 = 0x2;
								else l1 = 0;
							}
							else if (l2 == 0)
								l2 = 0x4;
							else l2 = 0;

							elem = l0 + l1 + l2;

							kbd_int_handler();
							timer_test_square(60);
							temp_counter = 0; //reset do temp_counter para voltar a contar os primeiros freq tiques
							timer_int_handler();
						}
					}
					/* process it */
					break;
				default:
					break; /* no other notifications expected: do nothing */
				}
			} else { /* received a standard message, not a notification */
				/* no standard messages expected: do nothing */
			}
		}
	}
	timer_unsubscribe_int();

	kbd_unsubscribe_int();

	////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}

int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}