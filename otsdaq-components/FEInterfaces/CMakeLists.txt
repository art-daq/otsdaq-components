include(otsdaq::FEInterface)

cet_make_library(LIBRARY_NAME FEOtsUDPProducerTemplateInterface SOURCE FEOtsUDPProducerTemplateInterfaceImpl.cc LIBRARIES PUBLIC otsdaq-components::DAQHardware otsdaq::FECore)
cet_build_plugin(FEOtsUDPProducerTemplateInterface otsdaq::FEInterface LIBRARIES REG otsdaq-components::FEOtsUDPProducerTemplateInterface)

cet_make_library(LIBRARY_NAME FEOtsUDPTemplateInterface SOURCE FEOtsUDPTemplateInterfaceImpl.cc LIBRARIES PUBLIC otsdaq-components::DAQHardware otsdaq::FECore)
cet_build_plugin(FEOtsUDPTemplateInterface otsdaq::FEInterface LIBRARIES REG  otsdaq-components::FEOtsUDPTemplateInterface  )

cet_make_library(LIBRARY_NAME FECommanderInterface SOURCE FECommanderInterfaceImpl.cc LIBRARIES PUBLIC otsdaq-components::DAQHardware otsdaq::FECore)
cet_build_plugin(FECommanderInterface otsdaq::FEInterface LIBRARIES REG  otsdaq-components::FECommanderInterface  )

cet_make_library(LIBRARY_NAME FESunrockInterface SOURCE FESunrockInterfaceImpl.cc LIBRARIES PUBLIC otsdaq::FECore)
cet_build_plugin(FESunrockInterface otsdaq::FEInterface LIBRARIES REG  otsdaq-components::FESunrockInterface  )


install_headers()
install_source()
