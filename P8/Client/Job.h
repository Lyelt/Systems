#ifndef JOB_H
#define JOB_H
class Job 
{
	public:
		Job() {}
		Job(short, short, short);
		Job(int, short, short, short);
		
		int id;
		static int idCount;
		int kidId;
		short speed;
		short fun;
		short difficulty;
		
		void setKidId(int);
		void print(ostream& out);
		int getPoints() const;
};
#endif