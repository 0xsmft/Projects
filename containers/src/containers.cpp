/**
 *
 * Module:
 *  containers
 *
 * Description:
 *  Container Library (Array, Vector, Queue...)
*/

#include <stdint.h>
#include <string>

// Static array
template<typename Ty, size_t N>
class Array
{
public:
	Array() = default;
	~Array() = default;

	constexpr void Fill( const Ty& rValue )
	{
		std::memset( &m_Data, rValue, N * sizeof( Ty ) );
	}

	constexpr Ty& Get( size_t index )
	{
		if( index >= N )
			__debugbreak();

		return m_Data[ index ];
	}

	constexpr const Ty& Get( size_t index ) const
	{
		if( index >= N )
			__debugbreak();

		return m_Data[ index ];
	}

	constexpr Ty& operator[]( size_t index )
	{
		return Get( index );
	}
	
	constexpr const Ty& operator[]( size_t index ) const
	{
		return Get( index );
	}

	constexpr Ty& Front()
	{
		return m_Data[ 0 ];
	}

	constexpr const Ty& Front() const
	{
		return m_Data[ 0 ];
	}

	constexpr Ty& Back()
	{
		return m_Data[ N - 1 ];
	}

	constexpr const Ty& Back() const
	{
		return m_Data[ N - 1 ];
	}

	constexpr Ty* Data()
	{
		return m_Data;
	}

	constexpr const Ty* Data() const
	{
		return m_Data;
	}

	constexpr size_t MaxSize() const
	{
		return N;
	}

	constexpr size_t Size() const
	{
		return N;
	}

private:
	Ty m_Data[ N ]{};
};


int main( int count, char** ppArgs )
{
	Array<uint8_t, 255> ByteArray;
	ByteArray.Fill( 0x43 );
	ByteArray[ 0 ] = 0xcc;


}
