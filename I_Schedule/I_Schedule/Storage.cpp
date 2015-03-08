#include "Storage.h"

//PUBLIC
Storage::Storage(){
	_filename = "default.txt";
	Load();
}

Storage::Storage(string filename){
	_filename = filename;
	Load();
}


Storage::~Storage(){
}
string Storage::Add(Task* task){
	taskList.push_back(task);
	string feedback = Rewrite();
	return feedback;
}

string Storage::Load(){
	try{
		ClearVectors();
		LoadFileContent();
		LoadTaskList();
	}
	catch (out_of_range){
		return _FEEDBACK_LOAD_FAILURE;
	}
	return _FEEDBACK_LOAD_SUCCESS;
}

string Storage::LoadFileContent(){
	string line;
	vector<string> all_lines;
	ifstream in(_filename.c_str());
	try{
		if (in.is_open()){
			while (getline(in, line)){
				if (line == _EMPTY_STRING){
					return _FEEDBACK_FILE_EMPTY;
				}
				//remove field description
				istringstream is(line);
				string buffer;
				is >> buffer;
				string output;
				getline(is, output);
				int length = line.length() - 1;
				output = output.substr(1, length);
				//end remove field description
				all_lines.push_back(output);
			}
			_filecontent = all_lines;
			return _FEEDBACK_LOAD_SUCCESS;
		}
	}
	catch (out_of_range){
		return _FEEDBACK_LOAD_FAILURE;
	}
	return _FEEDBACK_LOAD_FAILURE;
}

string Storage::LoadTaskList(){
	Smartstring str;
	const int startfield = Smartstring::FIELD::DESCRIPTION;
	int currentfield = startfield;
	int fieldcount = Smartstring::NUMBER_OF_FIELDS;
	vector<string>::iterator iter;
	Task* taskptr = new Task();
	try{
		for (iter = _filecontent.begin(); iter != _filecontent.end(); ++iter){
			if (currentfield == Smartstring::FIELD::DESCRIPTION){
				taskptr = new Task();
				taskList.push_back(taskptr);
			}
			switch (currentfield){
			case Smartstring::FIELD::DESCRIPTION:{
				taskptr->SetDescription(*iter);
				break;
			}
			case Smartstring::FIELD::STARTDATE:{
				taskptr->SetStartDate(*iter);
				break;
			}
			case Smartstring::FIELD::ENDDATE:{
				taskptr->SetEndDate(*iter);
				break;
			}
			case Smartstring::FIELD::PRIORITY:{
				taskptr->SetPriority(*iter);
				break;
			}

			}
			currentfield = (currentfield + 1) % fieldcount;
		}
	}
	
	catch (out_of_range){
		return _FEEDBACK_LOAD_FAILURE;
	}
	return _FEEDBACK_LOAD_SUCCESS;
}

string Storage::Rewrite(){
	ClearFile();
	string feedback = WriteVectors();
	return feedback;
}

string Storage::ToString(){
	ostringstream out;
	int index = 0;
	vector<Task*>::iterator iter;
	for (iter = taskList.begin(); iter != taskList.end(); ++iter){
		++index;
		if (iter + 1 != taskList.end()){
			out << index << ": " << (*iter)->GetDescription() << endl;
		}
		else{
			out << index << ": " << (*iter)->GetDescription();
		}
	}
	return out.str();
}

string Storage::Clear(){
	ClearFile();
	ClearVectors();
	return _FEEDBACK_CLEAR_SUCCESS;
}

string Storage::ClearFile(){
	ofstream out(_filename, ofstream::trunc);
	return _FEEDBACK_CLEAR_SUCCESS;
}
vector<string> Storage::GetContent(){
	return _filecontent;
}

string Storage::ClearVectors(){
	taskList.clear();
	_filecontent.clear();
	return _FEEDBACK_CLEAR_SUCCESS;
}

vector<Task*> Storage::GetTaskList(){
	return taskList;
}

string Storage::WriteVectors(){
	ostringstream out;
	ofstream of;
	of.open(_filename.c_str(), ios::app);
	vector<Task*>::iterator iter;
	try{
		for (iter = taskList.begin(); iter != taskList.end(); ++iter){
			out << (*iter)->ToString() << endl;
		}
		of << out.str();
	}
	catch (out_of_range){
		throw out_of_range(_FEEDBACK_WRITE_FAILURE);
		return _FEEDBACK_WRITE_FAILURE;
	}

	return _FEEDBACK_WRITE_SUCCESS;
}