#ifndef JOB_H
#define JOB_H
class Job 
{
	private:
		static int idCount;
		int kidId;
		short speed;
		short fun;
		short difficulty;
		
	public:
		Job() {}
		Job(short, short, short);
		int id;
		void setKidId(int);
		void print(ostream& out);
};
#endif