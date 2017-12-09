#ifndef JOB_H
#define JOB_H
class Job 
{
	public:
		Job();
		Job(short, short, short, const char*, bool);
		
		static int idCount;
		int id;
		bool done;
		const char* kidName;
		short speed;
		short fun;
		short difficulty;
		
		void setKidId(int);
		void print(ostream& out);
		int getPoints() const;
};
#endif