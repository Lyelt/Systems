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
		
		setKidId(int);
		print(ostream& out);
		
}
#endif