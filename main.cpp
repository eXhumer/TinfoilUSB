#include <libusb-1.0/libusb.h>
#include <iostream>
#include <iomanip>
#include "types.h"

int main(int argc, char* argv[])
{
	ssize_t dev_num = 0;
	int dev_config = 0;
	int xfer_length = 0;

	switch (libusb_init(NULL))
	{
	case LIBUSB_SUCCESS:
		libusb_device** devices;
		dev_num = libusb_get_device_list(NULL, &devices);
		for (int i = 0; i < dev_num; i++)
		{
			auto device = devices[i];

			libusb_device_descriptor dev_desc;
			libusb_get_device_descriptor(device, &dev_desc);

			/**
			* Nintendo Switch Known USB IDs (in hex)
			* | VID  | PID  |
			* | 16c0 | 27e2 | <- Tinfoil
			* | 057e | 3000 | <- Tinfoil / Tinleaf / Goldleaf / Awoo-Installer
			* | 057e | 2000 | <- Nintendo Switch (Normal Mode)
			* | 0955 | 7321 | <- Nintendo Switch (RCM Mode)
			*/
			switch (dev_desc.idVendor)
			{
			case 0x16c0: // Van Ooijen Technische Informatica
				switch (dev_desc.idProduct)
				{
				case 0x27e2: // Tinfoil (?)
					std::cout << "Tinfoil USB device detected!\n";
					libusb_device_handle* dev_handle;
					switch (libusb_open(device, &dev_handle))
					{
					case LIBUSB_SUCCESS:
						switch (libusb_get_configuration(dev_handle, &dev_config))
						{
						case LIBUSB_SUCCESS:
							if (dev_config == 0)
								switch (libusb_set_configuration(dev_handle, 1))
								{
								case LIBUSB_SUCCESS:
									std::cout << "Tinfoil USB Device unconfigured detected!\n";
									break;

								default:
									break;
								}
							break;

						default:
							break;
						}

						libusb_config_descriptor* p_conf_desc;
						switch (libusb_get_active_config_descriptor(device, &p_conf_desc))
						{
						case LIBUSB_SUCCESS:
							for (int dev_interface_idx = 0; dev_interface_idx < (int)p_conf_desc->bNumInterfaces; dev_interface_idx++)
							{
								auto p_dev_interface = &(p_conf_desc->interface[dev_interface_idx]);
								for (int interface_altsetting_idx = 0; interface_altsetting_idx < p_dev_interface->num_altsetting; interface_altsetting_idx++)
								{
									auto p_dev_interface_desc = &p_dev_interface->altsetting[interface_altsetting_idx];
									if ((libusb_class_code)p_dev_interface_desc->bInterfaceClass == LIBUSB_CLASS_VENDOR_SPEC)
										for (int endpoint_idx = 0; endpoint_idx < (int)p_dev_interface_desc->bNumEndpoints; endpoint_idx++)
										{
											auto p_dev_interface_ep_desc = &p_dev_interface_desc->endpoint[endpoint_idx];
											switch (p_dev_interface_ep_desc->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
											{
											case LIBUSB_ENDPOINT_IN:
												libusb_claim_interface(dev_handle, dev_interface_idx);
												tinfoil_usb_header_packet header_packet;
												libusb_bulk_transfer(dev_handle, p_dev_interface_ep_desc->bEndpointAddress,
													reinterpret_cast<unsigned char*>(&header_packet),
													sizeof(tinfoil_usb_header_packet), &xfer_length, 60000);
												if (header_packet.magic == TINFOIL_USB_HEADER_MAGIC)
												{
													print_header_packet(header_packet);
													char* payload_buffer = new char[header_packet.size + 1];
													libusb_bulk_transfer(dev_handle, p_dev_interface_ep_desc->bEndpointAddress,
														reinterpret_cast<unsigned char*>(payload_buffer), header_packet.size,
														&xfer_length, 60000);
													payload_buffer[header_packet.size] = 0;
													std::cout << "Payload Data: " << std::string(payload_buffer) << std::endl;
													delete[] payload_buffer;
												}
												libusb_release_interface(dev_handle, dev_interface_idx);
												break;
												
											case LIBUSB_ENDPOINT_OUT:
											default:
												break;
											}
										}
								}
							}
							break;

						default:
							break;
						}

						libusb_free_config_descriptor(p_conf_desc);
						libusb_close(dev_handle);
						break;

					default:
						break;
					}

					break;

				default:
					break;
				}
				break;

			case 0x057e: // Nintendo Co., Ltd
				switch (dev_desc.idProduct)
				{
				case 0x3000: // Tinfoil / Tinleaf / Goldleaf / Awoo-Installer
					break;

				case 0x2000: // Switch
					break;

				default:
					break;
				}
				break;

			case 0x0955: // NVIDIA
				switch (dev_desc.idProduct)
				{
				case 0x7321: // Switch RCM
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}
		}

		libusb_free_device_list(devices, 1);
		libusb_exit(NULL);
		break;

	default:
		break;
	}

	return 0;
}
