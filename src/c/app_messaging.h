//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once

enum REQUEST_TYPE {
  REQUEST_UNDEFINED = 0,
  REQUEST_CONFIG = 1,
  REQUEST_WEATHER = 2,
};

void send_request( enum REQUEST_TYPE requestType );
void messaging_init( void );
void messaging_deinit( void );
