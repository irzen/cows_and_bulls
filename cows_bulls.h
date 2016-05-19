
// just for convenience
template<int N>
struct in
{
	int val[N];

	int& operator[](int index);
	int operator[](int index) const;
};
typedef in<4> i4;
typedef in<2> i2;

i4 generateTask();		// take 4 random non-repetetive digits
i2 getFeedback(const i4, const i4);		// calc number of "cows" and "bulls"
bool validate(const i4);		// is number is correct? no dublicates?
