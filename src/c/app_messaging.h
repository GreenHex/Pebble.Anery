//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once

enum CMD_TYPE {
  CMD_UNDEFINED = 0,
  CMD_CONFIG = 1,
  CMD_WEATHER = 2,
};

void messaging_init( void );
void messaging_deinit( void );
void send_request( enum CMD_TYPE requestType );
