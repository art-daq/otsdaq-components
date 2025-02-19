cet_make_library(LIBRARY_NAME DAQHardware
SOURCE DAQHardwareBase.cc OtsUDPFirmwareCore.cc OtsUDPFirmwareDataGen.cc OtsUDPHardware.cc
		 LIBRARIES
		 PUBLIC
		 otsdaq::MessageFacility
		 otsdaq::NetworkUtilities
		 PRIVATE
		 otsdaq::BitManipulator
)


install_headers()
install_source()
