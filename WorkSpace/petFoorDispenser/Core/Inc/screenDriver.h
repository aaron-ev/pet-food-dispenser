/*
 * screenDriver.h
 *
 *  Created on: Jun 21, 2021
 *      Author: aaron
 */

#ifndef INC_SCREENDRIVER_H_
#define INC_SCREENDRIVER_H_


void screen_clear(void);
void screen_send_line(char *str,uint16_t *xy);
void screen_init(void);

#endif /* INC_SCREENDRIVER_H_ */
