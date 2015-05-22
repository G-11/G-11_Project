#pragma once
#ifndef _LIST_H_
#define _LIST_H_

//イテレーターもどき
template <typename TYPE>struct VALUE
{
	VALUE(){ _Next = _Prev = nullptr; Data = nullptr; Instance = false; }
	VALUE* _Next;
	VALUE* _Prev;
	TYPE* Data;
	bool Instance;
};

template <typename TYPE> class List
{
public:
	
	List()
	{
		_Begin = _End = nullptr;
		_Size = 0;
	}

	~List()
	{
		DeleteAll();
	}

	//リストに加える
	VALUE<TYPE>* Add(TYPE* data)
	{
		if (data == nullptr){ return nullptr; }
		VALUE<TYPE>* value = new VALUE<TYPE>;
		value->Data = data;
		if (_Begin == nullptr)
		{
			_Begin = _End = value;
		}
		else
		{
			_End->_Next = value;
			value->_Prev = _End;
			_End = value;
		}
		_Size++;

		return value;
	}

	//リストの最後尾に追加(ポインタ型ではない場合)
	VALUE<TYPE>* Add(TYPE data)
	{
		TYPE* type = new TYPE;
		*type = data;

		VALUE<TYPE>* value = new VALUE<TYPE>;
		value->Data = type;
		if (_Begin == nullptr)
		{
			_Begin = _End = value;
		}
		else
		{
			_End->_Next = value;
			value->_Prev = _End;
			_End = value;
		}
		value->Instance = true;
		_Size++;
		return value;
	}

	//リストから削除した後に解放(格納されたデータも消える)
	void Release(VALUE<TYPE>* value)
	{
		if (value == nullptr){ return; }
		Remove(value);
		delete value->Data;
		delete value;
		_Size--;
	}

	//リストからはずず
	void Delete(VALUE<TYPE>* value)
	{
		if (value == nullptr){ return ; }
		if (value->Instance)
		{
			Release(value);
		}
		else
		{
			Remove(value);
			delete value;
			_Size--;
		}
	}

	//全てリストから外す
	void DeleteAll(void)
	{
		VALUE<TYPE>* value = _Begin;
		VALUE<TYPE>* next = nullptr;
		while (value)
		{
			next = value->_Next;
			Delete(value);
			value = next;
		}
	}

	//リストの中身を全部消す
	void ReleaseAll(void)
	{
		VALUE<TYPE>* value = _Begin;
		VALUE<TYPE>* next = nullptr;
		while (value)
		{
			next = value->_Next;
			Release(value);
			value = next;
		}
		_Begin = _End = nullptr;
	}

	void Clear(void)
	{
		DeleteAll();
		_Begin = _End = nullptr;
	}

	VALUE<TYPE>* Begin(void){ return _Begin; }
	VALUE<TYPE>* End(void){ return _End; }

	int Size(void){ return _Size; }
private:
	//リストから削除
	void Remove(VALUE<TYPE>* value)
	{
		if (value == nullptr){ return; }
		if (value->_Prev != nullptr)
		{
			value->_Prev->_Next = value->_Next;
		}
		else
		{
			_Begin = value->_Next;
		}
		if (value->_Next != nullptr)
		{
			value->_Next->_Prev = value->_Prev;
		}
		else
		{
			_End = value->_Prev;
		}
		value->_Next = value->_Prev = nullptr;
	}

	VALUE<TYPE>* _Begin;
	VALUE<TYPE>* _End;

	int _Size;
};

#endif