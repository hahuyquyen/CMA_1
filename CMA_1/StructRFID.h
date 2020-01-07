
#ifndef STRUCTRFID_H
#define STRUCTRFID_H

	/////////////////////////////////////////////////////////////
	///// Data Tong Hop Task Checkdata           ///////////////
	///////////////////////////////////////////////////////////
	struct Data_TH {
		char id_RFID[25];
		char id_RFID_NV[25];
		double data_weight;
	};

	//////////////////////////////////////////////////////////////
	///// Data RFID           ///////////////////////////////////
	////////////////////////////////////////////////////////////
	struct Data_RFID {
		char id_RFID[25];
		char id_RFID_Old[25];
	};
#endif
