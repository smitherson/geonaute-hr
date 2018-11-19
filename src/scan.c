#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "gattlib.h"
#include <glib.h>
#include <sys/time.h>

FILE *output;

void notification_handler(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data) { 
  int i;
  struct timeval tv;
  gettimeofday(&tv,NULL);
 
  printf("HR: ");
  fprintf(output, "%d %d\n", tv.tv_sec, data[1]);
  printf( "%d %d ", tv.tv_sec, data[1]);
  fflush(output);
  printf("\n");                
} 

void print_services(gatt_connection_t* connection) {
  gattlib_primary_service_t* services;
  gattlib_characteristic_t* characteristics;
  int services_count, characteristics_count;

  char uuid_str[MAX_LEN_UUID_STR + 1];

  int ret = gattlib_discover_primary(connection, &services, &services_count);
  if (ret != 0) {
    fprintf(stderr, "Fail to discover primary services.\n");
    return ;
  }

  for (int i = 0; i < services_count; i++) {
    gattlib_uuid_to_string(&services[i].uuid, uuid_str, sizeof(uuid_str));

    printf("service[%d] start_handle:%02x end_handle:%02x uuid:%s\n", i,
        services[i].attr_handle_start, services[i].attr_handle_end,
        uuid_str);
  }
  free(services);

	ret = gattlib_discover_char(connection, &characteristics, &characteristics_count);
	if (ret != 0) {
		fprintf(stderr, "Fail to discover characteristics.\n");
		return ;
	}
	for (int i = 0; i < characteristics_count; i++) {
		gattlib_uuid_to_string(&characteristics[i].uuid, uuid_str, sizeof(uuid_str));

		printf("characteristic[%d] properties:%02x value_handle:%04x uuid:%s\n", i,
				characteristics[i].properties, characteristics[i].value_handle,
				uuid_str);
	}
	free(characteristics);
}

int main(int argc, char **argv)
{

  output = fopen("hearrate.log","w+");
  uint8_t buffer[100];

  gatt_connection_t* connection;
 
  connection = gattlib_connect(NULL, "E6:18:78:10:14:7B", BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
  if (connection == NULL) {
    printf("Fail to connect to the bluetooth device.\n");
    return 1;
  } else {
    printf("Connected to devicce!\n");
  }
  print_services(connection);

  uuid_t g_uuid;
  g_uuid.type=SDP_UUID16;
  g_uuid.value.uuid16=(0x2a37);

  gattlib_register_notification(connection, notification_handler, NULL);

  int ret = gattlib_notification_start(connection, &g_uuid);
  if (ret) {
    fprintf(stderr, "Fail to start notification\n.");
    return 1;
  }

  GMainLoop *loop = g_main_loop_new(NULL, 0);
  g_main_loop_run(loop);

  g_main_loop_unref(loop);
  gattlib_disconnect(connection); 
  puts("Done");

  return 0;
}

