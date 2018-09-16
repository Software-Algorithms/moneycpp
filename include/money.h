#pragma once
#include "currency.h"
#include "rounding.h"

namespace moneycpp
{

   template <typename TValue>
   struct money 
   {
      const TValue         amount;
      const currency_unit  currency;
   };

   template <typename TValue>
   inline bool operator==(money<TValue> const & lhs, money<TValue> const & rhs)
   {
      if (lhs.currency != rhs.currency) throw currency_mismatch_error();
      return lhs.amount == rhs.amount;
   }

   template <typename TValue>
   inline bool operator!=(money<TValue> const & lhs, money<TValue> const & rhs)
   {
      return !(lhs == rhs);
   }

   template <typename TValue>
   inline bool operator<(money<TValue> const & lhs, money<TValue> const & rhs)
   {
      if (lhs.currency != rhs.currency) throw currency_mismatch_error();
      return lhs.amount < rhs.amount;
   }

   template <typename TValue>
   inline bool operator>(money<TValue> const & lhs, money<TValue> const & rhs)
   {
      return rhs < lhs;
   }

   template <typename TValue>
   inline bool operator<=(money<TValue> const & lhs, money<TValue> const & rhs)
   {
      return !(rhs < lhs);
   }

   template <typename TValue>
   inline bool operator>=(money<TValue> const & lhs, money<TValue> const & rhs)
   {
      return !(lhs < rhs);
   }

   template <typename TValue1, typename TValue2>
   inline auto operator+(money<TValue1> const & lhs, money<TValue2> const & rhs)
   {
      if (lhs.currency != rhs.currency) throw currency_mismatch_error();
      return money<decltype(lhs.amount + rhs.amount)> {lhs.amount + rhs.amount, lhs.currency};
   }

   template <typename TValue1, typename TValue2>
   inline auto operator-(money<TValue1> const & lhs, money<TValue2> const & rhs)
   {
      if (lhs.currency != rhs.currency) throw currency_mismatch_error();
      return money<decltype(lhs.amount - rhs.amount)> {lhs.amount - rhs.amount, lhs.currency};
   }

   template <typename TValue1, typename TValue2>
   inline auto operator*(money<TValue1> const & lhs, TValue2 const rhs)
   {
      return money<decltype(lhs.amount * rhs)> {lhs.amount * rhs, lhs.currency};
   }

   template <typename TValue1, typename TValue2>
   inline auto operator/(money<TValue1> const & lhs, TValue2 const rhs)
   {
      if (rhs == 0) throw std::runtime_error("cannot divide by 0");
      return money<decltype(lhs.amount / rhs)> {lhs.amount / rhs, lhs.currency};
   }

   template <typename TValue>
   constexpr money<TValue> make_money(TValue const value, currency_unit const currency) noexcept
   {
      return money<TValue> { value, currency };
   }
   
   template <typename TValue>
   money<TValue> convert_money(money<TValue> const & m, currency_unit const currency, double const exchange_rate)
   {
      if(exchange_rate <= 0)
         throw std::runtime_error("Exchange rate must be positive");
      
      if(m.currency == currency)
         return m;
      
      return make_money<TValue>(m.amount * exchange_rate, currency);
   }

   namespace currency_literals
   {
      constexpr auto operator"" _usd(unsigned long long const value)
      {
         return make_money(value, currency::USD);
      }
   }
}
