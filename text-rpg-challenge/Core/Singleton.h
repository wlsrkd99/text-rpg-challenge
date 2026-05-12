#pragma once

namespace TextRPG
{
	/**
	 * @brief 싱글톤 패턴을 적용하기 위한 제네릭 템플릿 클래스 (CRTP 패턴)
	 * @tparam T 싱글톤으로 만들 클래스 타입
	 */
	template <typename T>
	class Singleton
	{
	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

	public:
		// 복사 생성자 및 대입 연산자 삭제 (복사 방지)
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		/** @brief 싱글톤 인스턴스를 반환합니다. */
		static T& GetInstance()
		{
			static T instance;
			return instance;
		}
	};
}