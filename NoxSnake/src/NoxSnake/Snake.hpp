#pragma once

#include "NoxSnake/Board.hpp"
#include "NoxSnake/Apple.hpp"

namespace NoxSnake {

	enum class Direction
	{
		None = 0,
		Down,
		Up,
		Left,
		Right
	};

	enum class State
	{
		Alive = 0,
		OutOfBounce,
		CollidesWithTail,

	};

	struct Segment
	{
		uint32_t x = 0;
		uint32_t y = 0;
		Direction NextMoveDir = Direction::None;
		Segment* Next = nullptr;
	};

	bool IsOccupiedBySnake(const uint32_t x, const uint32_t y, const Segment& head)
	{
		const Segment* current = &head;
		while (current)
		{
			if (x == current->x && y == current->y)
				return true;

			current = current->Next;
		}

		return false;
	}

	bool CollidesWithTail(const Segment& head)
	{
		const Segment* current = head.Next;
		while (current)
		{
			if (current->x == head.x && current->y == head.y)
				return true;

			current = current->Next;
		}

		return false;
	}

	State Move(Segment& head, const Board b, const Direction moveDir)
	{
		switch (moveDir)
		{
			case NoxSnake::Direction::Down:
			{
				if (head.y >= b.Height - 1)
					return State::OutOfBounce;
				
				head.y++;
				break;
			}

			case NoxSnake::Direction::Up:
			{
				if (head.y == 0)
					return State::OutOfBounce;
				
				head.y--;
				break;
			}

			case NoxSnake::Direction::Left:
			{
				if (head.x == 0)
					return State::OutOfBounce;
				
				head.x--;
				break;
			}

			case NoxSnake::Direction::Right:
			{
				if (head.x >= b.Width - 1)
					return State::OutOfBounce;
				
				head.x++;
				break;
			}
		}

		if (head.Next)
		{
			Move(*head.Next, b, head.Next->NextMoveDir);
			head.Next->NextMoveDir = moveDir;
		}

		if (CollidesWithTail(head))
			return State::CollidesWithTail;

		head.NextMoveDir = moveDir;

		return State::Alive;
	}

	void Grow(Segment& head)
	{
		if (head.Next)
		{
			Grow(*head.Next);
			return;
		}

		head.Next = new Segment{ head.x, head.y, Direction::None };
	}

	uint32_t Size(const Segment& head)
	{
		uint32_t size = 1;
		const Segment* current = head.Next;
		while (current)
		{
			size++;
			current = current->Next;
		}

		return size;
	}

	Direction MoveBruteForce(const Board b, const Segment& head)
	{
		if (head.y == 0)
			return head.x % 2 == 0 && head.x < b.Width - 1 ? Direction::Right : Direction::Down;

		if (head.y == b.Height - 2 && head.x < b.Width - 1)
		{
			if (head.x == b.Width - 1)
				return Direction::Down;
			
			return head.x % 2 == 0 ? Direction::Up : Direction::Right;
		}

		if (head.y == b.Height - 1)
			return head.x > 0 ? Direction::Left : Direction::Up;

		return head.NextMoveDir;
	}

	Direction MoveClosest(const Segment& head, const Apple apple)
	{
		Direction moveDir = Direction::None;
		
		const int32_t dx = head.x - apple.x;
		const int32_t dy = head.y - apple.y;

		const uint32_t distanceX = dx < 0 ? -dx : dx;
		const uint32_t distanceY = dy < 0 ? -dy : dy;
		
		if (distanceX > distanceY)
			moveDir = dx < 0 ? Direction::Right : Direction::Left;
		else
			moveDir = dy < 0 ? Direction::Down : Direction::Up;

		if (moveDir == Direction::Down && head.NextMoveDir == Direction::Up)
			moveDir = Direction::Left;

		if (moveDir == Direction::Up && head.NextMoveDir == Direction::Down)
			moveDir = Direction::Right;

		if (moveDir == Direction::Left && head.NextMoveDir == Direction::Right)
			moveDir = Direction::Up;

		if (moveDir == Direction::Right && head.NextMoveDir == Direction::Left)
			moveDir = Direction::Down;

		return moveDir;
	}


}