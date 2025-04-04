export default {
	'auth-settings-label': '機器認証設定',
	'no-mode-settings-text': '{{mode}}に関する設定項目はありません。',
	'settings-header-text': '入力モード設定',
	'gamepad-settings-header-text': 'コントローラ設定',
	'input-mode-label': '入力モード',
	'current-input-mode-label': '現在の入力モード',
	'input-mode-extra-label': 'Switch タッチパッドと Share',
	'input-mode-options': {
		none: 'モード未選択',
		'nintendo-switch': '任天堂Switch',
		generic: '標準HID',
		keyboard: 'キーボード',
		mdmini: 'Sega Genesis/メガドライブ Mini',
		pcemini: 'PCエンジン/Turbografx 16 Mini',
		astro: 'アストロシティミニ',
		psclassic: 'プレイステーションクラシック',
		xboxoriginal: '初代 Xbox',
	},
	'input-mode-group': {
		primary: '主要入力モード',
		mini: 'Mini/Classic コンソールモード',
	},
	'boot-input-mode-label': '起動時入力モード',
	'ps4-mode-options': {
		controller: 'コントローラ',
		arcadestick: 'アケコン',
	},
	'input-mode-authentication': {
		none: '無し',
		key: '認証鍵アップロード',
		usb: 'USBホスト',
		i2c: 'I2C接続',
	},
	'd-pad-mode-label': '十字キーモード',
	'd-pad-mode-options': {
		'd-pad': '十字キー',
		'left-analog': '左アナログ',
		'right-analog': '右アナログ',
	},
	'socd-cleaning-mode-label': 'SOCDクリーニングモード',
	'socd-cleaning-mode-note':
		'注：PS4、PS3、任天堂SwitchおよびミニコンソールシリーズのモードではSOCDクリーニングモード無効設定はできないため、ニュートラルSOCDクリーニングに設定されます。',
	'socd-cleaning-mode-options': {
		'up-priority': '上優先',
		neutral: 'ニュートラル',
		'last-win': '後入力優先',
		'first-win': '先入力優先',
		off: '無効',
	},
	'profile-label': 'プロファイル',
	'debounce-delay-label': 'チャタリング除去ディレイ(ミリ秒)',
	'mini-menu-gamepad-input': 'ディスプレイメニューの入力にゲームパッド入力を利用',
	'ps4-mode-explanation-text':
		'PS4モードはGP2040-CEコントローラを認証済みPS4コントローラとして動作させることができます。',
	'ps4-mode-warning-text':
		'<span>⏳ 警告 ⏳:</span> 認証無しの場合8分でPS4はタイムアウトして切断します.',
	'ps4-usb-host-mode-text':
		'<span>補足: </span> USB 周辺機器が有効化されており、PS4に互換性のあるデバイスが接続されていることを確認してください。',
	'ps4-id-mode-label': '識別モード',
	'ps4-id-mode-explanation-text':
		'<ul><li>コンソールモードはPS4接続時に利用してください。</li><li></li>リモート/エミュレーションモードはエミュレーションレイヤーとの接続やリモートプレイ環境でDualShock 4互換コントローラーが必要な際にのみ利用してください。</ul>',
	'ps4-id-mode-options': {
		console: 'コンソール',
		emulation: 'リモート/エミュレーション',
	},
	'ps5-mode-explanation-text':
		'PS5モードはGP2040-CEコントローラを認証済みPS5互換アケコンとして動作させることができます。',
	'ps5-mode-warning-text':
		'<span>⏳ 警告 ⏳:</span> 認証無しの場合8分でPS5はタイムアウトして切断します.',
	'ps5-usb-host-mode-text':
		'<span>補足: </span> USB 周辺機器が有効化されており、PS5に互換性のあるデバイスが接続されていることを確認してください。',
	'xbone-mode-text':
		'<span>補足: </span> XBox One モードで正しく認証するにははUSBホスト接続とUSBドングルが必要です。',
	'xinput-mode-text':
		'<span>補足:</span> Xinput モードはPCでは認証無しで利用可能です。Xbox 360本体で利用する場合は適切な認証用のUSBドングルを接続することで利用可能です。',
	'hotkey-settings-label': 'ホットキー設定',
	'hotkey-settings-sub-header':
		'<strong>Fn</strong>スライダーは<link_pinmap>端子割り当て設定</link_pinmap>ページで割り当て可能なファンクションボタン機能を有効化します。<strong>Fn</strong>スライダーを有効化することで、選択されたホットキーと同時にファンクションボタンを押す様に設定できます。<br />また、<strong>None</strong>を選択することでボタンの割り当てを解除できます。',
	'hotkey-settings-warning':
		'ファンクションボタンが未割り当てです。Fnスライダーは無効化されます。',
	'hotkey-actions': {
		'no-action': '未設定',
		'dpad-digital': '十字キー：デジタル',
		'dpad-left-analog': '十字キー：左アナログ',
		'dpad-right-analog': '十字キー：右アナログ',
		'home-button': 'Home ボタン',
		'capture-button': 'Capture ボタン',
		'socd-up-priority': 'SOCD 上優先',
		'socd-neutral': 'SOCD ニュートラル',
		'socd-last-win': 'SOCD 後入力優先',
		'socd-first-win': 'SOCD 先入力優先',
		'socd-off': 'SOCD クリーナ無効',
		'invert-x': '上下入力反転',
		'invert-y': '左右入力反転',
		'toggle-4way-joystick-mode': '4方向ジョイスティックモード切り替え',
		'toggle-ddi-4way-joystick-mode': 'DDI 4方向ジョイスティックモード切り替え',
		'b1-button': 'B1 ボタン',
		'b2-button': 'B2 ボタン',
		'b3-button': 'B3 ボタン',
		'b4-button': 'B4 ボタン',
		'l1-button': 'L1 ボタン',
		'r1-button': 'R1 ボタン',
		'l2-button': 'L2 ボタン',
		'r2-button': 'R2 ボタン',
		'l3-button': 'L3 ボタン',
		'r3-button': 'R3 ボタン',
		's1-button': 'S1 ボタン',
		's2-button': 'S2 ボタン',
		'a1-button': 'A1 ボタン',
		'a2-button': 'A2 ボタン',
		'a3-button': 'A3 ボタン',
		'a4-button': 'A4 ボタン',
		'dpad-up': '十字キー上',
		'dpad-down': '十字キー下',
		'dpad-left': '十字キー左',
		'dpad-right': '十字キー右',
		'touchpad-button': 'タッチパッドボタン',
		'load-profile-1': 'プロファイル #1をロード',
		'load-profile-2': 'プロファイル #2をロード',
		'load-profile-3': 'プロファイル #3をロード',
		'load-profile-4': 'プロファイル #4をロード',
		'reboot-default': 'GP2040-CEを再起動',
		'save-config': '設定を保存',
		'next-profile': '次のプロファイル',
		'previous-profile': '前のプロファイル',
		'menu-nav-up': 'メニュー 上',
		'menu-nav-down': 'メニュー 下',
		'menu-nav-left': 'メニュー 左',
		'menu-nav-right': 'メニュー 右',
		'menu-nav-select': 'メニュー 選択',
		'menu-nav-back': 'メニュー 戻る',
		'menu-nav-toggle': 'メニュー トグル',
	},
	'forced-setup-mode-label': '強制セットアップモード',
	'forced-setup-mode-options': {
		off: '無効',
		'disable-input-mode': '入力モードを無効化',
		'disable-web-config': 'Web設定モードを無効化',
		'disable-both': '両方無効化',
	},
	'forced-setup-mode-modal-title': '強制セットアップモードに関する注意',
	'forced-setup-mode-modal-body':
		'保存後にコントローラモードで再起動すると、以降Web設定モードにはアクセスできなくなります。注意点をよく理解したうえで、保存ボタンを有効化するには「{{warningCheckText}}」を以下に入力してください。閉じるボタンを押下した場合はこの設定は元に戻され保存されます。',
	'4-way-joystick-mode-label': '4方向ジョイスティックモード',
	'lock-hotkeys-label': 'ホットキーをロック',
	'keyboard-mapping-header-text': 'キーボードキー割り当て',
	'keyboard-mapping-sub-header-text':
		'以下のフォームを利用してボタンとキーの割り当て変更をしてください。',
	'usb-override': {
		'advanced-override': '⚠️ USBオーバーライド(上級者向け)',
		'invalid-warning-danger':
			'危険: 無効なUSBパラメータを設定するとデバイスが動作しなくなります。',
		'product-name': '製品名',
		manufacturer: '製造者',
		version: 'バージョン',
		'physical-warning-danger': '⚡ 物理ID変更 ⚡ ',
		'product-id': 'プロダクトID',
		'vendor-id': 'ベンダーID',
	},
};
