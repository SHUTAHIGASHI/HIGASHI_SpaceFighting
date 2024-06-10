#include "Player.h"
#include "Model.h"
#include "Shot.h"
#include "SceneMain.h"
#include "CameraController.h"
#include "UiManager.h"
#include "GameDataManager.h"
#include "EffekseerManager.h"
#include "SoundManager.h"
#include "UiManager.h"
#include "Load.h"

using namespace PlayerInfo;

namespace
{
	// 拡大率
	constexpr float kScale = 2.0f;
	// スタート位置座標
	constexpr float kStartPosX = 0.0f;
	constexpr float kStartPosY = -600.0f;
	constexpr float kStartPosZ = 0.0f;
	// プレイヤー半径
	constexpr float kPlayerRadius = 128.0f;
	// 移動の数値
	constexpr float kBaseSpeed = 32.0f;
	constexpr float kMoveSpeed = 24.0f;
	// プレイヤー傾きの数値
	constexpr float kAngleChangeAddNum = 0.03f;
	constexpr float kAngleChangeSubNum = 0.02f;
	constexpr float kAngleChangeMax = 0.5f;
	
	// HP回復量
	constexpr int kRepairNum = 2;

	// 無敵時間定数
	constexpr int kInvTimeMax = 60;
	// 攻撃遅延時間
	constexpr int kShotDelay = 6;

	// チャージショットの最大数
	constexpr int kChargeShotMax = 60;
	// チャージショット発射時間
	constexpr int kChargeShotTime = 10;
	// チャージショットのボタン押し時間
	constexpr int kChargeShotButtonDelay = 30;
	// チャージショットゲージ描画位置
	constexpr float kChargeShotDrawPosX = Game::kScreenWidthHalf;
	constexpr float kChargeShotDrawPosY = Game::kScreenHeightHalf + 300.0f;

	// 回避時間
	constexpr int kAvoidTime = 20;
	// 回避時の移動速度
	constexpr float kAvoidSpeed = 2.0f;
	// 回避時の回転速度
	constexpr float kAvoidRotaSpeed = 0.3f;
}

Player::Player() :
	m_updateFunc(&Player::NormalUpdate),
	m_hCrosshairImg(-1),
	m_hCrosshairImg2(-1),
	m_avoidDir(Game::kVecZero),
	m_shotDelay(0),
	m_chargeRate(0),
	m_chargeCount(0),
	m_chargeShotTime(0),
	m_chargeShotButtonDelay(0),
	m_avoidCount(0),
	m_avoidRotaSpeed(0.0f),
	m_playerRotaAxisZ(0.0f),
	m_isAvoid(false),
	m_isDrawCrosshair(false),
	m_pModel(nullptr),
	m_pCamera(nullptr),
	m_pScore(nullptr),
	m_pUi(nullptr)
{
	// 画像データ読込
	m_hCrosshairImg = LoadGraph("Data/ImageData/crosshair.png");
	m_hCrosshairImg2 = LoadGraph("Data/ImageData/crosshair_.png");
	// モデル読み込み
	m_pModel = std::make_shared<Model>(MV1DuplicateModel(Load::GetInstance().GetHandle("ship")));
}

Player::~Player()
{
	// モデルハンドル削除
	MV1DeleteModel(m_status.hModel);
	// クロスヘア画像ハンドル削除
	DeleteGraph(m_hCrosshairImg);
	DeleteGraph(m_hCrosshairImg2);
}

void Player::Init()
{
	// 座標初期化
	m_status.pos = VGet(kStartPosX, kStartPosY, kStartPosZ);
	// HP初期値代入
	m_status.hp = kHpMax;
	// プレイヤー移動速度初期化
	m_status.moveSpeed = kBaseSpeed;
	// プレイヤー方向初期化
	m_playerRotaAxisZ = 0.0f;
	// プレイヤーの当たり判定半径
	m_status.radius = kPlayerRadius;
	m_status.scale = kScale;

	// 座標設定
	m_pModel->SetPosition(m_status.pos);
	// 拡大率設定
	m_pModel->SetScale(m_status.scale);

	// エフェクト生成
	EffekseerManager::GetInstance().CreateEffect(EffectType::EngineFire, true, this);
}

void Player::Update(const InputState& input)
{
	// Update処理のメンバ関数ポインタ
	(this->*m_updateFunc)(input);
	// 弾の更新
	for (auto& shot : m_pShot)
	{
		shot->Update();
	}
	// 弾の削除
	DeleteDisableShot();

	// プレイヤーモデル描画位置設定
	m_pModel->SetPosition(m_status.pos);
	m_pModel->SetRotation(m_status.rota);
	// エフェクト更新
	EffekseerManager::GetInstance().SetEffectRota(EffectType::EngineFire, VGet(0.0f, 0.0f, m_playerRotaAxisZ));

	// UI更新
	m_pUi->SetPlayerHp(m_status.hp);
}

void Player::Draw()
{
	// プレイヤーモデル描画
	if (m_status.invTime % 2 == 0)
	{
		m_pModel->Draw();
	}

	// 弾の描画
	for (auto& shot : m_pShot)
	{
		shot->Draw();
	}

	// クロスヘア描画
	if (m_isDrawCrosshair)
	{
		DrawCrosshair();
	}
}

void Player::ControllMove(const InputState& input)
{
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_H)) OnDamage();
	if (CheckHitKey(KEY_INPUT_M)) m_status.invTime = 10000;
#endif
	// 常に前に進む
	VECTOR frontDir = VGet(0.0f, 0.0f, 1.0f);
	if (VSize(frontDir) > 0) frontDir = VNorm(frontDir);
	frontDir = VScale(frontDir, m_status.moveSpeed);
	m_status.pos = VAdd(m_status.pos, frontDir);

	// 移動キーの入力がされているか
	bool isSideMove = false, isRight = false;
	// 上下左右の移動処理
	m_status.dir = VGet(0.0f, 0.0f, 0.0f);

	if (!m_isAvoid)
	{
		if (input.IsPressed(InputType::up))		// 上方向
		{
			m_status.dir.y += 1.0f;
		}
		if (input.IsPressed(InputType::down))	// 下方向
		{
			m_status.dir.y -= 1.0f;
		}
		if (input.IsPressed(InputType::right))	// 右方向
		{
			m_status.dir.x += 1.0f;
			m_playerRotaAxisZ += -kAngleChangeAddNum;
			if (m_playerRotaAxisZ < -kAngleChangeMax)
			{
				m_playerRotaAxisZ = -kAngleChangeMax;
			}
			isSideMove = true;
			isRight = true;
		}
		if (input.IsPressed(InputType::left))	// 左方向
		{
			m_status.dir.x -= 1.0f;
			m_playerRotaAxisZ += kAngleChangeAddNum;
			if (m_playerRotaAxisZ > kAngleChangeMax)
			{
				m_playerRotaAxisZ = kAngleChangeMax;
			}
			isSideMove = true;
			isRight = false;
		}
	}

	// 移動方向の正規化
	if (VSize(m_status.dir) > 0) m_status.dir = VNorm(m_status.dir);
	// 移動方向に速度を掛ける
	m_status.dir = VScale(m_status.dir, kMoveSpeed);

	// 回避処理
	if (isSideMove)
	{
		if (input.IsTriggered(InputType::avoid) && !m_isAvoid)
		{
			// 回避中のフラグを立てる
			m_isAvoid = true;
			// 回避方向の設定
			m_avoidDir = m_status.dir;
			// 回避カウントを設定
			m_avoidCount = kAvoidTime;
			// 入力されている方向に回転
			if (isRight)
			{
				m_avoidRotaSpeed = -kAvoidRotaSpeed;
			}
			else
			{
				m_avoidRotaSpeed = kAvoidRotaSpeed;
			}
			// 回避エフェクト
			EffekseerManager::GetInstance().CreateEffect(EffectType::Avoid, false, this);
		}
	}

	// 回避中の場合
	if (m_isAvoid)
	{
		// 回避カウントを減らす
		m_avoidCount--;
		// 回避方向に速度を掛ける
		m_status.dir = VScale(m_avoidDir, kAvoidSpeed);
		// 回避時の回転を設定
		m_playerRotaAxisZ += m_avoidRotaSpeed;
		if(m_playerRotaAxisZ > DX_PI_F) m_playerRotaAxisZ = -DX_PI_F;
		if(m_playerRotaAxisZ < -DX_PI_F) m_playerRotaAxisZ = DX_PI_F;
		if (m_avoidCount == 1)
		{
			m_playerRotaAxisZ = 0.0f;
		}
		// 回避カウントが0以下になった場合
		if (m_avoidCount < 0)
		{
			// 回避中のフラグを下げる
			m_isAvoid = false;
		}
	}
	else
	{
		// 回避中でない場合は回避方向を初期化
		m_avoidDir = Game::kVecZero;
		m_avoidRotaSpeed = 0.0f;
	}
	// プレイヤーの座標を更新
	m_status.pos = VAdd(m_status.pos, m_status.dir);

	// 移動キーの入力がされていない場合
	if (!isSideMove && !m_isAvoid)
	{
		if (m_playerRotaAxisZ < -kAngleChangeSubNum)
		{
			m_playerRotaAxisZ += kAngleChangeSubNum;
		}
		else if (m_playerRotaAxisZ > kAngleChangeSubNum)
		{
			m_playerRotaAxisZ += -kAngleChangeSubNum;
		}
		else
		{
			m_playerRotaAxisZ = 0.0f;
		}
	}
	// プレイヤーの回転を設定
	m_status.rota = VGet(0.0f, 0.0f, m_playerRotaAxisZ);

	// プレイヤーの移動範囲を制限する
	CheckMoveLimit();
}

void Player::ControllShot(const InputState& input)
{
	// プレイヤーの攻撃処理
	if (input.IsTriggered(InputType::shot))
	{
		if (m_shotDelay < 0 && !m_isAvoid)
		{
			OnAttack();
			m_shotDelay = kShotDelay;
		}
	}

	// チャージショット処理
	if (input.IsPressed(InputType::shot))
	{
		// チャージショットボタン押下カウントを増やす
		m_chargeShotButtonDelay++;
		if (m_chargeShotButtonDelay > kChargeShotButtonDelay)
		{
			// チャージショットカウントを増やす
			m_chargeCount++;
			if (m_chargeCount >= kChargeShotMax)
			{
				m_chargeCount = kChargeShotMax;
			}
		}
	}
	else
	{
		// チャージショットカウントが一定数以上の場合
		if (m_chargeCount > kChargeShotMax * 0.3)
		{
			// チャージショット発射
			m_chargeShotTime = kChargeShotTime;
			m_chargeRate = m_chargeCount;
		}
		// チャージショットカウントを初期化
		m_chargeCount = 0;
		// チャージショットボタン押下カウントを初期化
		m_chargeShotButtonDelay = 0;
	}

	if (m_chargeShotTime > 0)
	{
		m_chargeShotTime--;
		// 回避中以外
		if (!m_isAvoid)
		{
			// チャージショット発射
			OnChargeAttack();
		}
	}

	// チャージショットUI
	if (m_chargeCount > 0)
	{
		// チャージショットUIがない場合は作成
		if (m_pUi->GetChargeShotUI() == nullptr)
		{
			m_pUi->CreateShotUI(kChargeShotMax);
		}
		// チャージショットUIの更新
		m_pUi->UpdateShotUI(m_chargeCount);
		m_pUi->SetShotDrawPos(kChargeShotDrawPosX, kChargeShotDrawPosX);
		// チャージエフェクト再生
		if (!EffekseerManager::GetInstance().IsPlayingEffect(EffectType::Charge))
		{
			EffekseerManager::GetInstance().CreateEffect(EffectType::Charge, false, this);
		}
	}
	else
	{
		// チャージショットUIがある場合は削除
		m_pUi->DeleteShotUI();
		// チャージエフェクト停止
		EffekseerManager::GetInstance().StopEffect(EffectType::Charge);
	}
}

void Player::OnDamage()
{
	if(m_avoidCount > 0) return;

	// 無敵時間のときは処理は行わない
	if (m_status.invTime > 0) return;
	// 無敵時間セット
	m_status.invTime = kInvTimeMax;

	// HPを減少させる
	m_status.hp--;
	if (m_status.hp < 0) m_status.hp = 0;

	// HPが0以上の場合
	if (m_status.hp >= 0)
	{
		// ダメージエフェクト再生
		EffekseerManager::GetInstance().CreateEffect(EffectType::Damage, false, this);
		// ダメージ音再生
		SoundManager::GetInstance().PlaySE(SoundType::damage);
		// 被ダメージ時のカメラ処理
		m_pCamera->OnScreenQuake();
		// 被ダメージ時のスコア減少
		GameDataManager::GetInstance().SubPoint(ScoreInfo::MISS_SCORE);
		// 被ダメージ時のUI処理
		m_pUi->OnLosePoint(ScoreInfo::MISS_SCORE);
	}
}

void Player::OnAttack()
{
	VECTOR target = VGet(m_status.pos.x, m_status.pos.y, m_status.pos.z + 100.0f);
	m_pShot.push_back(std::make_shared<Shot>(m_status.pos, target));
	// 攻撃音再生
	SoundManager::GetInstance().PlaySE(SoundType::shot);
	// 攻撃エフェクト再生
	EffekseerManager::GetInstance().CreateEffect(EffectType::Shot, false, m_pShot.back().get());
}

void Player::OnChargeAttack()
{
	// チャージショット拡大率
	float scale = static_cast<float>(m_chargeRate / 10);
	// チャージショットのターゲット座標
	VECTOR target = VGet(m_status.pos.x, m_status.pos.y, m_status.pos.z + 100.0f);
	// チャージショット生成
	m_pShot.push_back(std::make_shared<Shot>(m_status.pos, target));
	m_pShot.back()->SetScale(scale);
	// 攻撃音再生
	SoundManager::GetInstance().PlaySE(SoundType::shot);
	// 攻撃エフェクト再生
	EffekseerManager::GetInstance().CreateEffectAndSetScale(EffectType::ChargeShot, false, m_pShot.back().get(), scale);
}

void Player::OnDead()
{
	// 死亡音再生
	SoundManager::GetInstance().PlaySE(SoundType::dead);
	// 死亡判定をtrueに
	m_status.isDead = true;
	// 死亡時のエフェクト処理
	EffekseerManager::GetInstance().CreateEffect(EffectType::Dead, false, this);
	// メンバ関数ポインタをゲームオーバー後の処理に変更
	m_updateFunc = &Player::GameEndUpdate;
}

void Player::OnCollision(StageObjType type)
{
	// 敵や岩に当たった場合
	if (type == StageObjType::Rock || type == StageObjType::Enemy)
	{
		// 被ダメージ時
		OnDamage();
	}
	// アイテムに当たった場合
	else if (type == StageObjType::Item)
	{
		// 回復
		OnHeal();
	}
}

void Player::OnHeal()
{
	// HP回復
	m_status.hp += kRepairNum;
	if (m_status.hp > kHpMax) m_status.hp = kHpMax;
	// 回復エフェクト再生
	EffekseerManager::GetInstance().CreateEffect(EffectType::Heal, false, this);
	// 回復音再生
	SoundManager::GetInstance().PlaySE(SoundType::boost);
	// 回復時のスコア加算
	GameDataManager::GetInstance().AddPoint(ScoreInfo::HEAL_SCORE);
	// 回復時のUI処理
	m_pUi->OnGetPoint(ScoreInfo::HEAL_SCORE);
}

void Player::CheckMoveLimit()
{
	// プレイヤーの移動範囲を制限する
	if (m_status.pos.x > Game::kStageWidth) m_status.pos.x = Game::kStageWidth;
	if (m_status.pos.x < -Game::kStageWidth) m_status.pos.x = -Game::kStageWidth;
	if (m_status.pos.y > Game::kStageHeight) m_status.pos.y = Game::kStageHeight;
	if (m_status.pos.y < -Game::kStageHeight) m_status.pos.y = -Game::kStageHeight;
}

void Player::CheckDead()
{
	// 無敵時間ではない
	if (m_status.invTime <= 0)
	{
		// HPが0以下
		if (m_status.hp <= 0)
		{
			// 死亡していない判定
			if (!m_status.isDead)
			{
				// 死亡処理を行う
				OnDead();
			}
		}
	}
}

void Player::DrawCrosshair()
{
	// 手前のクロスヘア
	VECTOR pos = VGet(m_status.pos.x, m_status.pos.y, m_status.pos.z + 800.0f);
	pos = ConvWorldPosToScreenPos(pos);
	DrawRotaGraphF(pos.x, pos.y, 1.0f, 0.0f, m_hCrosshairImg2, true);
	// 奥のクロスヘア
	pos = VGet(m_status.pos.x, m_status.pos.y, m_status.pos.z + 6500.0f);
	pos = ConvWorldPosToScreenPos(pos);
	DrawRotaGraphF(pos.x, pos.y, 1.0f, 0.0f, m_hCrosshairImg, true);
}

void Player::DeleteDisableShot()
{
	// いなくなったショットは排除
	auto rmIt = std::remove_if(m_pShot.begin(), m_pShot.end(),
		[](const std::shared_ptr<Shot>& shot)
		{
			return !shot->IsEnabled();
		});

	// 実際に範囲を指定して削除
	m_pShot.erase(rmIt, m_pShot.end());
}

void Player::NormalUpdate(const InputState& input)
{
	// 無敵時間が残っている場合は毎フレームマイナスする
	if (m_status.invTime > 0) m_status.invTime--;

	// 攻撃遅延時間を減らす
	m_shotDelay--;

	// 死亡判定チェック
	CheckDead();

	// 基本の移動処理
	ControllMove(input);

	// 攻撃処理
	ControllShot(input);
}

void Player::GameEndUpdate(const InputState& input)
{
	// 常に前に進む
	m_status.dir = VGet(0.0f, 0.0f, 1.0f);
	if (VSize(m_status.dir) > 0) m_status.dir = VNorm(m_status.dir);
	float playerMoveSpeed = m_status.moveSpeed;
	m_status.dir = VScale(m_status.dir, playerMoveSpeed / 2);
	m_status.pos = VAdd(m_status.pos, m_status.dir);
	// プレイヤーの座標を更新
	EffekseerManager::GetInstance().SetEffectPosition(EffectType::Dead, m_status.pos);
}
