#ifndef COMPONENT_H
#define COMPONENT_H
class IComponent
{
public:
	virtual int Initialize(void) = 0;
	virtual int Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Finalize(void) = 0;
};
#endif // !COMPONENT_H