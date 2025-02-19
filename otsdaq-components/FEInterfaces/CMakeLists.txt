include(otsdaq::FEInterface)

cet_make_library(LIBRARY_NAME FEOtsUDPProducerTemplateInterface SOURCE FEOtsUDPProducerTemplateInterfaceImpl.cc LIBRARIES PUBLIC otsdaq_components::DAQHardware otsdaq::FECore)
cet_build_plugin(FEOtsUDPProducerTemplateInterface otsdaq::FEInterface LIBRARIES REG otsdaq_components::FEOtsUDPProducerTemplateInterface)

cet_make_library(LIBRARY_NAME FEOtsUDPTemplateInterface SOURCE FEOtsUDPTemplateInterfaceImpl.cc LIBRARIES PUBLIC otsdaq_components::DAQHardware otsdaq::FECore)
cet_build_plugin(FEOtsUDPTemplateInterface otsdaq::FEInterface LIBRARIES REG  otsdaq_components::FEOtsUDPTemplateInterface  )

cet_make_library(LIBRARY_NAME FECommanderInterface SOURCE FECommanderInterfaceImpl.cc LIBRARIES PUBLIC otsdaq_components::DAQHardware otsdaq::FECore)
cet_build_plugin(FECommanderInterface otsdaq::FEInterface LIBRARIES REG  otsdaq_components::FECommanderInterface  )


install_headers()
install_source()
