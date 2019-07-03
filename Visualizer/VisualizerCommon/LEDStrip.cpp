
#include "LEDStrip.h"


#include <fstream>
#include <iostream>
#include <string.h>

//Defining thread function types for Linux

#define THREAD static void*
#define THREADRETURN return(NULL);

//Include pthread and Unix standard libraries for building on Linux

#include "pthread.h"
#include "unistd.h"


LEDStrip::LEDStrip()
{
    num_leds = 25;
}


LEDStrip::~LEDStrip()
{
}

THREAD lsthread(void *param)
{
    LEDStrip* led = static_cast<LEDStrip*>(param);
    led->LEDStripUpdateThread();
    THREADRETURN
}

void LEDStrip::Initialize(int led_type, char* ledstring, int matrix_size, int matrix_pos, int sections, int rotate_x, bool mirror_x, bool mirror_y, bool single_color)
{
    this->led_type = led_type;

    if (led_type == LED_STRIP_NORMAL)
    {
        //InitializeHuePlus(ledstring);
        strcpy(led_string, ledstring);

        //Assume serial device unless a different protocol is specified
        bool    serial = TRUE;
        bool	udp = FALSE;

        LPSTR   numleds = NULL;
        LPSTR   source = NULL;
        LPSTR   udpport_baud = NULL;
        LPSTR   next = NULL;
        LPSTR   apikey = NULL;

        source = strtok_s(ledstring, ",", &next);

        //Check if we are setting up a Visualizer UDP protocol device
        if (strncmp(source, "udp:", 4) == 0)
        {
            source = source + 4;
            serial = FALSE;
            udp = TRUE;

        }

        //Check for either the UDP port or the serial baud rate
        if (strlen(next))
        {
            udpport_baud = strtok_s(next, ",", &next);
        }

        //Check for the number of LEDs
        if (strlen(next))
        {
            numleds = strtok_s(next, ",", &next);
        }



        if (serial)
        {
            if (udpport_baud == NULL)
            {
                //Initialize with default baud rate
                //InitializeSerial(source, 115200);
            }
            else
            {
                //Initialize with custom baud rate
                //InitializeSerial(source, atoi(udpport_baud));
            }
        }
        else
        {
            if (udpport_baud == NULL)
            {
                //Do something
            }
            else
            {
                if (udp)
                {
                    //Initialize UDP port
                    InitializeUDP(source, udpport_baud);
                }
            }
        }

        if (numleds != NULL && strlen(numleds))
        {
            SetNumLEDs(atoi(numleds), matrix_size, matrix_pos, sections, rotate_x, mirror_x, mirror_y, single_color);
        }
    }


	pthread_t threads[1];

	pthread_create(&threads[0], NULL, &lsthread, this);

}

void LEDStrip::InitializeUDP(char * clientname, char * port)
{
    strcpy(client_name, clientname);
    strcpy(port_name, port);

    udpport = new net_port(client_name, port_name);

}

char* LEDStrip::GetLEDString()
{
    return(led_string);
}

void LEDStrip::SetNumLEDs(int numleds, int matrix_size, int matrix_pos, int sections, int rotate_x, bool mirror_x, bool mirror_y, bool single_color)
{
    int y_index = ROW_IDX_BAR_GRAPH;

    num_leds = numleds;

    LEDStripXIndex = new int[num_leds];
    LEDStripYIndex = new int[num_leds];

    if ((num_leds % 2) == 0)
    {
        //for vertical strips
        //for (int section = 0; section < sections; section++)
        //{
        //    //Even number of LEDs
        //    for (int i = 0; i < (num_leds / sections); i++)
        //    {
        //        int led_idx = (section * (num_leds / sections)) + i;
        //        int matrix_pos_adj = matrix_pos + section;

        //        LEDStripXIndex[led_idx] = (int)((section * (256.0f / (sections - 1))));

        //        if (sections == ((num_leds / sections) - 1))
        //        {
        //            LEDStripXIndex[led_idx] = LEDStripXIndex[led_idx] - 1;
        //        }

        //        if (matrix_size > 0)
        //        {
        //            y_index = (int)(ROW_IDX_SPECTROGRAPH_TOP + ((i)* (SPECTROGRAPH_ROWS / (num_leds / sections))) + (0.5f * (SPECTROGRAPH_ROWS / (num_leds / sections))));
        //        }

        //        LEDStripYIndex[led_idx] = y_index;
        //    }
        //}
        //Even number of LEDs

        //For horizontal strips
        for (int section = 0; section < sections; section++)
        {
            //Even number of LEDs
            for (int i = 0; i < (num_leds / sections); i++)
            {
                int x_index        = i + rotate_x;
                int led_idx        = (section * (num_leds / sections)) + i;
                int matrix_pos_adj = matrix_pos + section;

                if (x_index >= (num_leds / sections))
                {
                    x_index = (num_leds / sections) - x_index;
                }

                if (x_index < 0)
                {
                    x_index = (num_leds / sections) + x_index;
                }

                if (mirror_x)
                {
                    LEDStripXIndex[led_idx] = (int)(num_leds / sections) - ((x_index * (256.0f / ((num_leds / sections) - 1))));
                }
                else
                {
                    LEDStripXIndex[led_idx] = (int)((x_index * (256.0f / ((num_leds / sections) - 1))));
                }
                
                if (x_index == ((num_leds / sections) - 1))
                {
                    LEDStripXIndex[led_idx] = LEDStripXIndex[led_idx] - 1;
                }

                if (matrix_size > 0)
                {
                    if (single_color)
                    {
                        y_index = ROW_IDX_SINGLE_COLOR;
                    }
                    else
                    {
                        if (mirror_y)
                        {
                            y_index = (int)(ROW_IDX_SPECTROGRAPH_TOP + (((matrix_pos + sections - 1) - (matrix_pos + section - 1)) * (SPECTROGRAPH_ROWS / matrix_size)) + (0.5f * (SPECTROGRAPH_ROWS / matrix_size)));
                        }
                        else
                        {
                            y_index = (int)(ROW_IDX_SPECTROGRAPH_TOP + ((matrix_pos + section - 1) * (SPECTROGRAPH_ROWS / matrix_size)) + (0.5f * (SPECTROGRAPH_ROWS / matrix_size)));
                        }
                    }
                }

                LEDStripYIndex[led_idx] = y_index;
            }
        }
    }
    else
    {
        //Odd number of LEDs
        for (int i = 0; i < num_leds; i++)
        {
            sections = 1;
            int x_index = i + rotate_x;

            if (x_index >= (num_leds / sections))
            {
                x_index = (num_leds / sections) - x_index;
            }

            if (x_index < 0)
            {
                x_index = (num_leds / sections) + x_index;
            }

            if (single_color)
            {
                LEDStripYIndex[i] = ROW_IDX_SINGLE_COLOR;
            }
            else
            {
                LEDStripYIndex[i] = y_index;
            }

            if (x_index == (num_leds / 2))
            {
                LEDStripXIndex[i] = 128;
            }
            else if (x_index < ((num_leds / 2) + 1))
            {
                LEDStripXIndex[i] = (num_leds / 2) + ((x_index + 1) * (256 / (num_leds + 1)));
            }
            else
            {
                LEDStripXIndex[i] = ((num_leds / 2) + 1) + (x_index * (256 / (num_leds + 1)));
            }
        }
    }
}

void LEDStrip::SetLEDs(COLORREF pixels[64][256])
{
    if (udpport != NULL)
    {
        unsigned char *serial_buf;

        serial_buf = new unsigned char[(num_leds * 3) + 3];

        serial_buf[0] = 0xAA;

        for (int idx = 0; idx < (num_leds * 3); idx += 3)
        {
            int pixel_idx = idx / 3;
            COLORREF color = pixels[LEDStripYIndex[pixel_idx]][LEDStripXIndex[pixel_idx]];
            serial_buf[idx + 1] = GetRValue(color);
            serial_buf[idx + 2] = GetGValue(color);
            serial_buf[idx + 3] = GetBValue(color);
        }

        unsigned short sum = 0;

        for (int i = 0; i < (num_leds * 3) + 1; i++)
        {
            sum += serial_buf[i];
        }

        serial_buf[(num_leds * 3) + 1] = sum >> 8;
        serial_buf[(num_leds * 3) + 2] = sum & 0x00FF;

        if (udpport != NULL)
        {
            udpport->udp_write((char *)serial_buf, (num_leds * 3) + 3);
        }

        delete[] serial_buf;
    }
}

void LEDStrip::SetPixels(COLORREF pixels[64][256])
{
    led_pixels = pixels;
}

void LEDStrip::SetDelay(int delay)
{
    led_delay = delay;
}

void LEDStrip::LEDStripUpdateThread()
{
    while (TRUE)
    {
        if (led_pixels != NULL)
        {
            switch (led_type)
            {
            case LED_STRIP_NORMAL:
                SetLEDs(led_pixels);
                break;

            }

            Sleep(led_delay);
        }
    }
}
