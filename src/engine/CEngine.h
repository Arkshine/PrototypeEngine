#ifndef ENGINE_CENGINE_H
#define ENGINE_CENGINE_H

class CEngine final
{
public:
	CEngine() = default;

	bool Run( const bool bIsListenServer );

private:
	CEngine( const CEngine& ) = delete;
	CEngine& operator=( const CEngine& ) = delete;
};

#endif //ENGINE_CENGINE_H
