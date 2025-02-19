#cet_make(LIBRARY_NAME UserTableCore
#		 LIBRARIES
#         FECore
#         cetlib
#)

#install_headers()
#install_source()

include(otsdaq::table)
cet_build_plugin(FENIMPlusInterfaceTable otsdaq::table  )

cet_build_plugin(FEOtsUDPFSSRInterfaceTable otsdaq::table)

cet_build_plugin(FEOtsUDPLCLS2InterfaceTable otsdaq::table)

cet_build_plugin(FEOtsUDPTemplateInterfaceTable otsdaq::table  )

cet_build_plugin(FEPurdueFSSRInterfaceTable otsdaq::table  )

cet_build_plugin(FEPurdueNetworkMasterInterfaceTable otsdaq::table  )

cet_build_plugin(FSSRDACsTable otsdaq::table  )
