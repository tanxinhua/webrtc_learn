# IntervalBudget

## WebRTC IntervalBudget分析
### 转载自 [https://zhuanlan.zhihu.com/p/184944074](https://zhuanlan.zhihu.com/p/184944074)

WebRTC中Pacing等模块需要按照指定的码率发送报文，保证码率稳定，会用到`IntervalBudget`这个类。本篇将介绍`IntervalBudget`这个类。

### 1. ✨**IntervalBudget原理**

`IntervalBudget`顾名思义，就是一段时间内的发送码率预算。 `IntervalBudget`根据时间流逝增加budget，报文发送后减少budget，每次发送报文前判断剩余budget是否足够，如果不足则取消本次发送。

举个例子 :

* 当前目标码率设置为1000kbps，假设剩余budget为100bytes，因为窗口kWindowMs=500ms，所以最大的budget限制在`1000*500/8=6000bytes`。
* 距离上次更新相隔50ms，那么budget就多了`1000kbps*50/8=600bytes`， 剩余`100+600=700bytes`；
* 如果当前发送一个1000bytes的报文，先判断是否有剩余budget，当前700bytes肯定有剩余，因此可以发送，并减少budget，剩余`700-1000=-300bytes`。

  
`IntervalBudget`这个类比较小，因此这里直接贴上其声明：

```text
class IntervalBudget {
 public:
  explicit IntervalBudget(int initial_target_rate_kbps);
  IntervalBudget(int initial_target_rate_kbps, bool can_build_up_underuse);
  // 设置目标发送码率
  void set_target_rate_kbps(int target_rate_kbps);

  // 时间流逝后增加budget
  void IncreaseBudget(int64_t delta_time_ms);
  // 发送数据后减少budget
  void UseBudget(size_t bytes);

  // 剩余budget
  size_t bytes_remaining() const;
  // 剩余budget占当前窗口数据量比例
  double budget_ratio() const;
  // 目标发送码率
  int target_rate_kbps() const;

 private:
  // 设置的目标码率，按照这个码率控制数据发送
  int target_rate_kbps_;
  // 窗口内（500ms）对应的最大字节数=窗口大小*target_rate_kbps_/8
  int64_t max_bytes_in_budget_;
  // 剩余可发送字节数，限制范围:[-max_bytes_in_budget_, max_bytes_in_budget_]
  int64_t bytes_remaining_;
  // 上个周期underuse，本周期是否可以借用上个周期的剩余量
  bool can_build_up_underuse_;
};
```

### 2. ✨**budget增加**

如果距离上次更新时间相隔`delta_time_ms`，那么随着时间流逝，那么这段时间增长的budget为`delta_time_ms* target_rate_kbps_`：

```text
void IntervalBudget::IncreaseBudget(int64_t delta_time_ms) {
  int64_t bytes = target_rate_kbps_ * delta_time_ms / 8;
  if (bytes_remaining_ < 0 || can_build_up_underuse_) {
    // We overused last interval, compensate this interval.
    // 如果上次发送的过多（bytes_remaining_ < 0），那么本次发送的数据量会变少
    bytes_remaining_ = std::min(bytes_remaining_ + bytes, max_bytes_in_budget_);
  } else {
    // If we underused last interval we can't use it this interval.
    // 1） 如果上次的budget没有用完（bytes_remaining_ > 0），如果没有设置can_build_up_underuse_
    // 不会对上次的补偿，而是开始新的一轮

    // 2） 如果设置了can_build_up_underuse_标志，那意味着要考虑上次的underuse，
    // 如果上次没有发送完，则本次需要补偿，见上面if逻辑
    bytes_remaining_ = std::min(bytes, max_bytes_in_budget_);
  }
}
```

### 3. ✨**减少budget**

发送了数据后需要减少budget，直接减去发送字节数即可：

```text
void IntervalBudget::UseBudget(size_t bytes) {
  bytes_remaining_ = std::max(bytes_remaining_ - static_cast<int>(bytes),
                              -max_bytes_in_budget_);
}
```

### 4. ✨谈谈IntervalBudget窗口大小

目前窗口大小kWindowMs = 500ms，从代码中可以看到，可以通过这个窗口，限制当前剩余码率不会超过500ms窗口的数据，避免过多历史数据累计影响，将影响控制在500ms内。比如，上次发送的报文过大，剩余budget是个负数，窗口太大导致剩余budget没有限制可以很小，导致`IncreaseBudget`要很长时间才能补偿上次的超发；如果之前发送过少，且开启了can\_build\_up\_underuse\_，窗口太大导致剩余budget没有限制可以很大，后续余量将很足，容易超发。

