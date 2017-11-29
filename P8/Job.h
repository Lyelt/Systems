#ifndef JOB_H
#define JOB_H
class Job 
{
	private:
		int kidId;
		int id;
		short speed;
		short fun;
		short difficulty;
		
	public:
		Job() {}
		Job(short, short, short);
		
		void setKidId(int);
		void print(ostream& out);
		
};
#endif